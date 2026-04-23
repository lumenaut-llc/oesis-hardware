#include <Arduino.h>
#include <time.h>

#if __has_include(<WiFi.h>)
#include <WiFi.h>
#define FLOOD_NODE_HAS_WIFI 1
#else
#define FLOOD_NODE_HAS_WIFI 0
#endif

#if __has_include("secrets.h")
#include "secrets.h"
#endif

#ifndef FLOOD_NODE_WIFI_SSID
#define FLOOD_NODE_WIFI_SSID ""
#endif

#ifndef FLOOD_NODE_WIFI_PASSWORD
#define FLOOD_NODE_WIFI_PASSWORD ""
#endif

static const char* kSchemaVersion = "oesis.flood-node.v1";
static const char* kNodeId = "flood-node-01";
static const char* kFirmwareVersion = "0.1.0";
static const char* kLocationMode = "outdoor";
static const char* kInstallRole = "runoff_low_point";
static const char* kObservedAtPlaceholder = "1970-01-01T00:00:00Z";
static const char* kWifiSsid = FLOOD_NODE_WIFI_SSID;
static const char* kWifiPassword = FLOOD_NODE_WIFI_PASSWORD;
static const char* kNtpServer = "pool.ntp.org";
static const long kGmtOffsetSeconds = 0;
static const int kDaylightOffsetSeconds = 0;

static const int kAnalogPin = 4;
static const float kAdcReferenceVoltage = 3.3f;
static const int kAdcMaxCount = 4095;
static const float kVoltageDividerRatio = 2.0f;
static const float kDistanceCmPerSensorVolt = 100.0f;
static const float kDistanceCmOffset = 0.0f;
static const float kDryReferenceDistanceCm = 70.0f;
static const unsigned long kSampleIntervalMs = 5000;
static const unsigned long kWifiConnectTimeoutMs = 15000;
static const unsigned long kTimeSyncTimeoutMs = 15000;

struct Mb7389Reading {
  bool present;
  int analog_raw;
  float sensor_voltage_v;
  float distance_cm;
};

unsigned long last_sample_ms = 0;
unsigned long read_failures_total = 0;
String last_error = "boot";
bool wifi_connected = false;
bool time_synced = false;
String observed_at = kObservedAtPlaceholder;
float previous_water_depth_cm = 0.0f;
unsigned long previous_depth_ms = 0;

float adcToBoardVoltage(int raw) {
  return (static_cast<float>(raw) / kAdcMaxCount) * kAdcReferenceVoltage;
}

float boardToSensorVoltage(float board_voltage) {
  return board_voltage * kVoltageDividerRatio;
}

float sensorVoltageToDistanceCm(float sensor_voltage) {
  return (sensor_voltage * kDistanceCmPerSensorVolt) + kDistanceCmOffset;
}

float distanceToWaterDepthCm(float distance_cm) {
  float depth = kDryReferenceDistanceCm - distance_cm;
  return depth > 0.0f ? depth : 0.0f;
}

void printJsonString(const String& value) {
  Serial.print('"');
  for (size_t i = 0; i < value.length(); ++i) {
    char c = value.charAt(i);
    if (c == '"' || c == '\\') {
      Serial.print('\\');
    }
    Serial.print(c);
  }
  Serial.print('"');
}

void printFloat(float value, int digits = 1) {
  Serial.print(value, digits);
}

void updateObservedAt() {
  if (!time_synced) {
    observed_at = kObservedAtPlaceholder;
    return;
  }
  struct tm time_info;
  if (!getLocalTime(&time_info)) {
    observed_at = kObservedAtPlaceholder;
    time_synced = false;
    last_error = "time_read_failed";
    return;
  }
  char buffer[25];
  strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", &time_info);
  observed_at = buffer;
}

void syncTimeIfConfigured() {
  if (strlen(kWifiSsid) == 0) {
    Serial.println("# Wi-Fi not configured, using placeholder observed_at");
    return;
  }
#if !FLOOD_NODE_HAS_WIFI
  last_error = "wifi_unavailable";
  Serial.println("# Wi-Fi support unavailable in this build, using placeholder observed_at");
  return;
#else
  WiFi.mode(WIFI_STA);
  WiFi.begin(kWifiSsid, kWifiPassword);
  unsigned long wifi_start_ms = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - wifi_start_ms < kWifiConnectTimeoutMs) {
    delay(250);
  }
  wifi_connected = WiFi.status() == WL_CONNECTED;
  if (!wifi_connected) {
    last_error = "wifi_connect_failed";
    Serial.println("# Wi-Fi connect failed, using placeholder observed_at");
    return;
  }
  configTime(kGmtOffsetSeconds, kDaylightOffsetSeconds, kNtpServer);
  unsigned long time_start_ms = millis();
  while (!time_synced && millis() - time_start_ms < kTimeSyncTimeoutMs) {
    struct tm time_info;
    if (getLocalTime(&time_info, 250)) {
      time_synced = true;
      break;
    }
  }
  if (!time_synced) {
    last_error = "ntp_sync_failed";
    Serial.println("# NTP sync failed, using placeholder observed_at");
    return;
  }
  updateObservedAt();
#endif
}

Mb7389Reading readMb7389() {
  Mb7389Reading reading = {false, 0, 0.0f, 0.0f};
  int raw = analogRead(kAnalogPin);
  if (raw < 0 || raw > kAdcMaxCount) {
    last_error = "adc_read_invalid";
    return reading;
  }
  float board_voltage = adcToBoardVoltage(raw);
  float sensor_voltage = boardToSensorVoltage(board_voltage);
  reading.present = true;
  reading.analog_raw = raw;
  reading.sensor_voltage_v = sensor_voltage;
  reading.distance_cm = sensorVoltageToDistanceCm(sensor_voltage);
  if (!isfinite(reading.distance_cm) || reading.distance_cm < 0.0f) {
    reading.present = false;
    last_error = "distance_invalid";
  }
  return reading;
}

float computeRiseRateCmPerHr(float water_depth_cm) {
  unsigned long now = millis();
  if (previous_depth_ms == 0 || now <= previous_depth_ms) {
    previous_water_depth_cm = water_depth_cm;
    previous_depth_ms = now;
    return 0.0f;
  }
  float elapsed_hours = static_cast<float>(now - previous_depth_ms) / 3600000.0f;
  if (elapsed_hours <= 0.0f) {
    previous_water_depth_cm = water_depth_cm;
    previous_depth_ms = now;
    return 0.0f;
  }
  float rise_rate = (water_depth_cm - previous_water_depth_cm) / elapsed_hours;
  previous_water_depth_cm = water_depth_cm;
  previous_depth_ms = now;
  return rise_rate;
}

void emitPacket(const Mb7389Reading& mb7389, float water_depth_cm, float rise_rate_cm_per_hr) {
  Serial.print("{\"schema_version\":\"");
  Serial.print(kSchemaVersion);
  Serial.print("\",\"node_id\":\"");
  Serial.print(kNodeId);
  Serial.print("\",\"observed_at\":\"");
  Serial.print(observed_at);
  Serial.print("\",\"firmware_version\":\"");
  Serial.print(kFirmwareVersion);
  Serial.print("\",\"location_mode\":\"");
  Serial.print(kLocationMode);
  Serial.print("\",\"install_role\":\"");
  Serial.print(kInstallRole);
  Serial.print("\",\"sensors\":{\"mb7389\":{\"present\":");
  Serial.print(mb7389.present ? "true" : "false");
  Serial.print(",\"analog_raw\":");
  Serial.print(mb7389.analog_raw);
  Serial.print(",\"sensor_voltage_v\":");
  printFloat(mb7389.sensor_voltage_v, 2);
  Serial.print(",\"distance_cm\":");
  printFloat(mb7389.distance_cm);
  Serial.print("}},\"derived\":{\"dry_reference_distance_cm\":");
  printFloat(kDryReferenceDistanceCm);
  Serial.print(",\"water_depth_cm\":");
  printFloat(water_depth_cm);
  Serial.print(",\"rise_rate_cm_per_hr\":");
  printFloat(rise_rate_cm_per_hr);
  Serial.print(",\"calibration_state\":\"provisional\"},\"health\":{\"uptime_s\":");
  Serial.print(millis() / 1000UL);
  Serial.print(",\"wifi_connected\":");
  Serial.print(wifi_connected ? "true" : "false");
  Serial.print(",\"free_heap_bytes\":");
#ifdef ESP32
  Serial.print(ESP.getFreeHeap());
#else
  Serial.print(0);
#endif
  Serial.print(",\"read_failures_total\":");
  Serial.print(read_failures_total);
  Serial.print(",\"last_error\":");
  if (last_error.length() == 0) {
    Serial.print("null");
  } else {
    printJsonString(last_error);
  }
  Serial.print("}}");
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  delay(250);
  analogReadResolution(12);
  Serial.println("# flood_node_serial_json");
  Serial.print("# analog pin: GPIO");
  Serial.println(kAnalogPin);
  syncTimeIfConfigured();
  last_error = "";
}

void loop() {
  unsigned long now = millis();
  if (now - last_sample_ms < kSampleIntervalMs) {
    delay(50);
    return;
  }
  last_sample_ms = now;

  Mb7389Reading mb7389 = readMb7389();
  if (!mb7389.present) {
    read_failures_total++;
  }
  float water_depth_cm = distanceToWaterDepthCm(mb7389.distance_cm);
  float rise_rate_cm_per_hr = computeRiseRateCmPerHr(water_depth_cm);
  updateObservedAt();
  emitPacket(mb7389, water_depth_cm, rise_rate_cm_per_hr);
}
