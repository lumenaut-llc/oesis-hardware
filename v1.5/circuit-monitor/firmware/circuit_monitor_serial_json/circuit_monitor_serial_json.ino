// Circuit monitor serial JSON emitter for ESP32-S3.
// Reads one or two PZEM energy monitoring modules over Modbus RTU,
// classifies equipment state from current-draw thresholds, tracks
// cycles, and prints one JSON packet per line at 115200.
// See serial-json-contract.md for the target packet shape.

#include <Arduino.h>
#include <PZEM004Tv30.h>

#if __has_include(<WiFi.h>)
#include <WiFi.h>
#define CM_HAS_WIFI 1
#else
#define CM_HAS_WIFI 0
#endif

#if __has_include("config.h")
#include "config.h"
#else
// Defaults if no config.h — see config.example.h
#define CM_NODE_ID "circuit-monitor-01"
#define CM_FIRMWARE_VERSION "0.1.0"
#define CM_PZEM_RX_PIN 16
#define CM_PZEM_TX_PIN 17
#define CM_CIRCUIT_0_ENABLED true
#define CM_CIRCUIT_0_ID "hvac_main"
#define CM_CIRCUIT_0_TYPE "central_ac_120v"
#define CM_CIRCUIT_0_ADDRESS 0x01
#define CM_CIRCUIT_0_SAMPLE_INTERVAL_MS 10000
#define CM_CIRCUIT_0_IDLE_MAX_A 0.3
#define CM_CIRCUIT_0_FAN_MIN_A 2.0
#define CM_CIRCUIT_0_FAN_MAX_A 4.0
#define CM_CIRCUIT_0_COMPRESSOR_MIN_A 8.0
#define CM_CIRCUIT_0_COMPRESSOR_MAX_A 15.0
#define CM_CIRCUIT_0_OVERLOAD_A 15.0
#define CM_CIRCUIT_1_ENABLED false
#define CM_CIRCUIT_1_ID "sump_primary"
#define CM_CIRCUIT_1_TYPE "sump_pump_120v"
#define CM_CIRCUIT_1_ADDRESS 0x02
#define CM_CIRCUIT_1_SAMPLE_INTERVAL_MS 5000
#define CM_CIRCUIT_1_IDLE_MAX_A 0.2
#define CM_CIRCUIT_1_RUNNING_MIN_A 3.0
#define CM_CIRCUIT_1_RUNNING_MAX_A 8.0
#define CM_CIRCUIT_1_OVERLOAD_A 8.0
#define CM_WIFI_SSID ""
#define CM_WIFI_PASSWORD ""
#endif

static const char* kSchemaVersion = "oesis.circuit-monitor.v1";
static const char* kObservedAtPlaceholder = "1970-01-01T00:00:00Z";

// --- Circuit state tracking ---

struct CircuitState {
  bool enabled;
  const char* circuit_id;
  const char* circuit_type;
  uint8_t address;
  unsigned long sample_interval_ms;
  unsigned long last_sample_ms;
  bool responding;
  float current_a;
  float power_w;
  float voltage_v;
  float power_factor;
  float energy_kwh;
  const char* inferred_state;
  bool cycle_active;
  unsigned long cycle_start_ms;
  unsigned long cycle_count;
  unsigned long read_failures;
};

CircuitState circuits[2];
PZEM004Tv30* pzem[2] = {nullptr, nullptr};

unsigned long read_failures_total = 0;
bool wifi_connected = false;
bool time_synced = false;
String observed_at = kObservedAtPlaceholder;
String last_error = "boot";
unsigned long packet_seq = 0;

// --- HVAC state classification ---

const char* classifyHvac(float current_a) {
  if (current_a <= CM_CIRCUIT_0_IDLE_MAX_A) return "off";
  if (current_a >= CM_CIRCUIT_0_FAN_MIN_A && current_a <= CM_CIRCUIT_0_FAN_MAX_A) return "fan_only";
  if (current_a >= CM_CIRCUIT_0_COMPRESSOR_MIN_A && current_a <= CM_CIRCUIT_0_COMPRESSOR_MAX_A) return "compressor_running";
  if (current_a > CM_CIRCUIT_0_OVERLOAD_A) return "overload";
  return "unknown";
}

// --- Sump pump state classification ---

const char* classifySump(float current_a) {
  if (current_a <= CM_CIRCUIT_1_IDLE_MAX_A) return "standby";
  if (current_a >= CM_CIRCUIT_1_RUNNING_MIN_A && current_a <= CM_CIRCUIT_1_RUNNING_MAX_A) return "running";
  if (current_a > CM_CIRCUIT_1_OVERLOAD_A) return "overload";
  // Brief spike between idle and running range
  if (current_a > CM_CIRCUIT_1_IDLE_MAX_A && current_a < CM_CIRCUIT_1_RUNNING_MIN_A) return "starting";
  return "unknown";
}

const char* classifyState(int idx, float current_a) {
  if (idx == 0) return classifyHvac(current_a);
  return classifySump(current_a);
}

bool isIdleState(const char* state) {
  return strcmp(state, "off") == 0 || strcmp(state, "standby") == 0;
}

// --- Time sync ---

void syncTimeIfConfigured() {
  if (strlen(CM_WIFI_SSID) == 0) return;
#if !CM_HAS_WIFI
  last_error = "wifi_unavailable";
  return;
#else
  WiFi.mode(WIFI_STA);
  WiFi.begin(CM_WIFI_SSID, CM_WIFI_PASSWORD);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) delay(250);
  wifi_connected = WiFi.status() == WL_CONNECTED;
  if (!wifi_connected) { last_error = "wifi_connect_failed"; return; }
  configTime(0, 0, "pool.ntp.org");
  start = millis();
  while (!time_synced && millis() - start < 15000) {
    struct tm ti;
    if (getLocalTime(&ti, 250)) { time_synced = true; break; }
  }
  if (!time_synced) last_error = "ntp_sync_failed";
#endif
}

void updateObservedAt() {
  if (!time_synced) { observed_at = kObservedAtPlaceholder; return; }
  struct tm ti;
  if (!getLocalTime(&ti)) { observed_at = kObservedAtPlaceholder; time_synced = false; return; }
  char buf[25];
  strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &ti);
  observed_at = buf;
}

// --- Read and classify ---

void readCircuit(int idx) {
  CircuitState& c = circuits[idx];
  if (!c.enabled || pzem[idx] == nullptr) return;

  unsigned long now = millis();
  if (now - c.last_sample_ms < c.sample_interval_ms) return;
  c.last_sample_ms = now;

  float v = pzem[idx]->voltage();
  if (isnan(v)) {
    c.responding = false;
    c.read_failures++;
    read_failures_total++;
    last_error = String("pzem_read_failed_0x0") + String(c.address, HEX);
    return;
  }

  c.responding = true;
  c.voltage_v = v;
  c.current_a = pzem[idx]->current();
  c.power_w = pzem[idx]->power();
  c.power_factor = pzem[idx]->pf();
  c.energy_kwh = pzem[idx]->energy();

  const char* prev_state = c.inferred_state;
  c.inferred_state = classifyState(idx, c.current_a);

  // Cycle tracking
  bool was_idle = isIdleState(prev_state);
  bool is_idle = isIdleState(c.inferred_state);

  if (was_idle && !is_idle) {
    c.cycle_active = true;
    c.cycle_start_ms = now;
  } else if (!was_idle && is_idle && c.cycle_active) {
    c.cycle_active = false;
    c.cycle_count++;
  }
}

// --- JSON emission ---

void printJsonString(const char* s) {
  Serial.print('"');
  Serial.print(s);
  Serial.print('"');
}

void emitCircuit(int idx) {
  CircuitState& c = circuits[idx];
  Serial.print("{\"circuit_id\":"); printJsonString(c.circuit_id);
  Serial.print(",\"current_a\":"); Serial.print(c.current_a, 2);
  Serial.print(",\"power_w\":"); Serial.print(c.power_w, 1);
  Serial.print(",\"voltage_v\":"); Serial.print(c.voltage_v, 1);
  Serial.print(",\"power_factor\":"); Serial.print(c.power_factor, 2);
  Serial.print(",\"energy_kwh\":"); Serial.print(c.energy_kwh, 2);
  Serial.print(",\"inferred_state\":"); printJsonString(c.inferred_state);
  Serial.print(",\"cycle_active\":"); Serial.print(c.cycle_active ? "true" : "false");
  Serial.print(",\"cycle_duration_s\":");
  if (c.cycle_active) {
    Serial.print((millis() - c.cycle_start_ms) / 1000UL);
  } else {
    Serial.print("null");
  }
  Serial.print("}");
}

void emitPacket() {
  updateObservedAt();
  packet_seq++;

  Serial.print("{\"schema_version\":"); printJsonString(kSchemaVersion);
  Serial.print(",\"node_id\":"); printJsonString(CM_NODE_ID);
  Serial.print(",\"firmware_version\":"); printJsonString(CM_FIRMWARE_VERSION);
  Serial.print(",\"uptime_s\":"); Serial.print(millis() / 1000UL);
  Serial.print(",\"observed_at\":\""); Serial.print(observed_at); Serial.print("\"");
  Serial.print(",\"circuits\":[");

  bool first = true;
  for (int i = 0; i < 2; i++) {
    if (!circuits[i].enabled || !circuits[i].responding) continue;
    if (!first) Serial.print(",");
    emitCircuit(i);
    first = false;
  }

  Serial.print("],\"health\":{");
  Serial.print("\"wifi_rssi\":");
#if CM_HAS_WIFI
  if (wifi_connected) { Serial.print(WiFi.RSSI()); } else { Serial.print("null"); }
#else
  Serial.print("null");
#endif
  Serial.print(",\"heap_free\":");
#ifdef ESP32
  Serial.print(ESP.getFreeHeap());
#else
  Serial.print(0);
#endif
  Serial.print(",\"sample_interval_ms\":"); Serial.print(CM_CIRCUIT_0_SAMPLE_INTERVAL_MS);
  Serial.print(",\"read_failures_total\":"); Serial.print(read_failures_total);
  Serial.print(",\"last_error\":");
  if (last_error.length() == 0) Serial.print("null");
  else { printJsonString(last_error.c_str()); }
  Serial.print("}}");
  Serial.println();
}

// --- Setup and loop ---

void setup() {
  Serial.begin(115200);
  delay(250);
  Serial.println("# circuit_monitor_serial_json");
  Serial.print("# UART2 pin plan: RX=GPIO"); Serial.print(CM_PZEM_RX_PIN);
  Serial.print(" TX=GPIO"); Serial.println(CM_PZEM_TX_PIN);

  syncTimeIfConfigured();

  // Initialize circuit 0
  circuits[0] = {CM_CIRCUIT_0_ENABLED, CM_CIRCUIT_0_ID, CM_CIRCUIT_0_TYPE,
                 CM_CIRCUIT_0_ADDRESS, CM_CIRCUIT_0_SAMPLE_INTERVAL_MS,
                 0, false, 0, 0, 0, 0, 0, "off", false, 0, 0, 0};
  if (circuits[0].enabled) {
    pzem[0] = new PZEM004Tv30(Serial2, CM_PZEM_RX_PIN, CM_PZEM_TX_PIN, CM_CIRCUIT_0_ADDRESS);
    float v = pzem[0]->voltage();
    circuits[0].responding = !isnan(v);
    Serial.print("# PZEM address 0x0"); Serial.print(CM_CIRCUIT_0_ADDRESS, HEX);
    Serial.println(circuits[0].responding ? ": responding" : ": NOT responding");
  }

  // Initialize circuit 1
  circuits[1] = {CM_CIRCUIT_1_ENABLED, CM_CIRCUIT_1_ID, CM_CIRCUIT_1_TYPE,
                 CM_CIRCUIT_1_ADDRESS, CM_CIRCUIT_1_SAMPLE_INTERVAL_MS,
                 0, false, 0, 0, 0, 0, 0, "standby", false, 0, 0, 0};
  if (circuits[1].enabled) {
    pzem[1] = new PZEM004Tv30(Serial2, CM_PZEM_RX_PIN, CM_PZEM_TX_PIN, CM_CIRCUIT_1_ADDRESS);
    float v = pzem[1]->voltage();
    circuits[1].responding = !isnan(v);
    Serial.print("# PZEM address 0x0"); Serial.print(CM_CIRCUIT_1_ADDRESS, HEX);
    Serial.println(circuits[1].responding ? ": responding" : ": NOT responding");
  }

  if (last_error == "boot") last_error = "";
}

void loop() {
  readCircuit(0);
  readCircuit(1);

  // Emit at the slower cadence
  static unsigned long last_emit_ms = 0;
  unsigned long now = millis();
  if (now - last_emit_ms >= CM_CIRCUIT_0_SAMPLE_INTERVAL_MS) {
    last_emit_ms = now;
    emitPacket();
  }

  delay(50);
}
