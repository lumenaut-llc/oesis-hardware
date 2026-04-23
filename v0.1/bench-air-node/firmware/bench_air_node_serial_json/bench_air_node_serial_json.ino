// Bench air node packet emitter for ESP32-S3.
// Uses SHT45 and BME680 over I2C on GPIO8/GPIO9 and prints one
// JSON packet per line for local ingest validation.
// Optional: HTTP POST the same JSON to a reference ingest service over Wi-Fi.

#include <Arduino.h>
#include <Wire.h>
#include <time.h>
#include <Adafruit_BME680.h>
#include <Adafruit_SHT4x.h>
#include <Adafruit_Sensor.h>

#if __has_include(<WiFi.h>)
#include <WiFi.h>
#include <HTTPClient.h>
#define BENCH_AIR_HAS_WIFI 1
#else
#define BENCH_AIR_HAS_WIFI 0
#endif

#if __has_include("secrets.h")
#include "secrets.h"
#endif

#ifndef BENCH_AIR_WIFI_SSID
#define BENCH_AIR_WIFI_SSID ""
#endif

#ifndef BENCH_AIR_WIFI_PASSWORD
#define BENCH_AIR_WIFI_PASSWORD ""
#endif

#ifndef BENCH_AIR_INGEST_URL
#define BENCH_AIR_INGEST_URL ""
#endif

#ifndef BENCH_AIR_PARCEL_ID
#define BENCH_AIR_PARCEL_ID "parcel_demo_001"
#endif

static const char* kSchemaVersion = "oesis.bench-air.v1";
static const char* kNodeId = "bench-air-01";
static const char* kFirmwareVersion = "0.1.0";
static const char* kLocationMode = "indoor";
static const char* kObservedAtPlaceholder = "1970-01-01T00:00:00Z";
static const char* kWifiSsid = BENCH_AIR_WIFI_SSID;
static const char* kWifiPassword = BENCH_AIR_WIFI_PASSWORD;
static const char* kIngestUrl = BENCH_AIR_INGEST_URL;
static const char* kParcelId = BENCH_AIR_PARCEL_ID;
static const char* kNtpServer = "pool.ntp.org";
static const long kGmtOffsetSeconds = 0;
static const int kDaylightOffsetSeconds = 0;

static const int kI2cSdaPin = 8;
static const int kI2cSclPin = 9;
static const unsigned long kSampleIntervalMs = 5000;
static const unsigned long kWifiConnectTimeoutMs = 15000;
static const unsigned long kTimeSyncTimeoutMs = 15000;
static const uint16_t kHttpPostTimeoutMs = 12000;  // HTTPClient::setTimeout is uint16_t on ESP32
static const uint8_t kBme680Addresses[] = {0x77, 0x76};

struct Sht45Reading {
  bool present;
  float temperature_c;
  float relative_humidity_pct;
};

struct Bme680Reading {
  bool present;
  float temperature_c;
  float relative_humidity_pct;
  float pressure_hpa;
  float gas_resistance_ohm;
};

unsigned long last_sample_ms = 0;
unsigned long read_failures_total = 0;
bool sht45_present = false;
bool bme680_present = false;
uint8_t bme680_address = 0;
String last_error = "boot";
bool wifi_connected = false;
bool time_synced = false;
String observed_at = kObservedAtPlaceholder;

Adafruit_SHT4x sht4 = Adafruit_SHT4x();
Adafruit_BME680 bme680;

Sht45Reading readSht45() {
  Sht45Reading reading;
  reading.present = false;
  reading.temperature_c = 0.0f;
  reading.relative_humidity_pct = 0.0f;

  if (!sht45_present) {
    return reading;
  }

  sensors_event_t humidity_event;
  sensors_event_t temperature_event;
  sht4.getEvent(&humidity_event, &temperature_event);

  if (isnan(temperature_event.temperature) || isnan(humidity_event.relative_humidity)) {
    last_error = "sht45_read_failed";
    return reading;
  }

  reading.present = true;
  reading.temperature_c = temperature_event.temperature;
  reading.relative_humidity_pct = humidity_event.relative_humidity;
  return reading;
}

Bme680Reading readBme680() {
  Bme680Reading reading;
  reading.present = false;
  reading.temperature_c = 0.0f;
  reading.relative_humidity_pct = 0.0f;
  reading.pressure_hpa = 0.0f;
  reading.gas_resistance_ohm = 0.0f;

  if (!bme680_present) {
    return reading;
  }

  if (!bme680.performReading()) {
    last_error = "bme680_read_failed";
    return reading;
  }

  reading.present = true;
  reading.temperature_c = bme680.temperature;
  reading.relative_humidity_pct = bme680.humidity;
  reading.pressure_hpa = bme680.pressure / 100.0f;
  reading.gas_resistance_ohm = bme680.gas_resistance;
  return reading;
}

void appendJsonEscaped(String& s, const String& value) {
  s += '"';
  for (size_t i = 0; i < value.length(); ++i) {
    char c = value.charAt(i);
    if (c == '"' || c == '\\') {
      s += '\\';
    }
    s += c;
  }
  s += '"';
}

void appendOptionalFloat(String& s, bool has_value, float value, int digits) {
  if (!has_value) {
    s += "null";
    return;
  }
  s += String(value, digits);
}

String buildPacketJson(const Sht45Reading& sht45, const Bme680Reading& bme680) {
  float derived_temperature_c = sht45.present ? sht45.temperature_c : bme680.temperature_c;
  float derived_relative_humidity_pct = sht45.present ? sht45.relative_humidity_pct : bme680.relative_humidity_pct;
  bool has_primary_temperature = sht45.present || bme680.present;
  bool has_primary_humidity = sht45.present || bme680.present;

  String out;
  out.reserve(1800);

  out += "{\"schema_version\":\"";
  out += kSchemaVersion;
  out += "\",\"node_id\":\"";
  out += kNodeId;
  out += "\",\"observed_at\":\"";
  out += observed_at;
  out += "\",\"firmware_version\":\"";
  out += kFirmwareVersion;
  out += "\",\"location_mode\":\"";
  out += kLocationMode;
  out += "\",\"sensors\":{\"sht45\":{\"present\":";
  out += sht45.present ? "true" : "false";
  out += ",\"temperature_c\":";
  out += String(sht45.temperature_c, 1);
  out += ",\"relative_humidity_pct\":";
  out += String(sht45.relative_humidity_pct, 1);
  out += "},\"bme680\":{\"present\":";
  out += bme680.present ? "true" : "false";
  out += ",\"temperature_c\":";
  out += String(bme680.temperature_c, 1);
  out += ",\"relative_humidity_pct\":";
  out += String(bme680.relative_humidity_pct, 1);
  out += ",\"pressure_hpa\":";
  out += String(bme680.pressure_hpa, 1);
  out += ",\"gas_resistance_ohm\":";
  out += String(bme680.gas_resistance_ohm, 0);
  out += "}},\"derived\":{\"temperature_c_primary\":";
  appendOptionalFloat(out, has_primary_temperature, derived_temperature_c, 1);
  out += ",\"relative_humidity_pct_primary\":";
  appendOptionalFloat(out, has_primary_humidity, derived_relative_humidity_pct, 1);
  out += ",\"pressure_hpa\":";
  appendOptionalFloat(out, bme680.present, bme680.pressure_hpa, 1);
  out += ",\"voc_trend_source\":";
  if (bme680.present) {
    out += "\"gas_resistance_ohm\"";
  } else {
    out += "null";
  }
  out += "},\"health\":{\"uptime_s\":";
  out += String(millis() / 1000UL);
  out += ",\"wifi_connected\":";
  out += wifi_connected ? "true" : "false";
  out += ",\"free_heap_bytes\":";
#ifdef ESP32
  out += String(ESP.getFreeHeap());
#else
  out += "0";
#endif
  out += ",\"read_failures_total\":";
  out += String(read_failures_total);
  out += ",\"last_error\":";
  if (last_error.length() == 0) {
    out += "null";
  } else {
    appendJsonEscaped(out, last_error);
  }
  out += "}}";
  return out;
}

void emitPacketToSerial(const String& json_line) {
  Serial.println(json_line);
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

bool beginBme680() {
  for (size_t i = 0; i < sizeof(kBme680Addresses) / sizeof(kBme680Addresses[0]); ++i) {
    uint8_t candidate = kBme680Addresses[i];
    if (bme680.begin(candidate, &Wire)) {
      bme680_address = candidate;
      bme680.setTemperatureOversampling(BME68X_OS_8X);
      bme680.setHumidityOversampling(BME68X_OS_2X);
      bme680.setPressureOversampling(BME68X_OS_4X);
      bme680.setIIRFilterSize(BME68X_FILTER_SIZE_3);
      bme680.setGasHeater(320, 150);
      return true;
    }
  }
  return false;
}

void connectWifiAndSyncNtp() {
#if !BENCH_AIR_HAS_WIFI
  wifi_connected = false;
  time_synced = false;
  if (strlen(kIngestUrl) > 0) {
    last_error = "wifi_unavailable";
    Serial.println("# HTTP ingest disabled: Wi-Fi not available in this build");
  } else {
    Serial.println("# Wi-Fi not configured, using placeholder observed_at");
  }
  return;
#else
  if (strlen(kWifiSsid) == 0) {
    wifi_connected = false;
    time_synced = false;
    Serial.println("# Wi-Fi not configured, using placeholder observed_at");
    if (strlen(kIngestUrl) > 0) {
      Serial.println("# HTTP ingest disabled: set BENCH_AIR_WIFI_SSID in secrets.h");
    }
    return;
  }

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

  Serial.print("# Wi-Fi connected, IP=");
  Serial.println(WiFi.localIP());

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
  Serial.print("# time synced: ");
  Serial.println(observed_at);

  if (strlen(kIngestUrl) > 0) {
    Serial.println("# HTTP ingest enabled for POST /v1/ingest/node-packets");
  }
#endif
}

#if BENCH_AIR_HAS_WIFI
void ensureWifiForPost() {
  if (strlen(kIngestUrl) == 0 || strlen(kWifiSsid) == 0) {
    return;
  }
  if (WiFi.status() == WL_CONNECTED) {
    wifi_connected = true;
    return;
  }

  wifi_connected = false;
  WiFi.mode(WIFI_STA);
  WiFi.begin(kWifiSsid, kWifiPassword);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < kWifiConnectTimeoutMs) {
    delay(200);
  }

  wifi_connected = WiFi.status() == WL_CONNECTED;
  if (!wifi_connected) {
    last_error = "wifi_reconnect_failed";
  }
}

void clearHttpLastErrorIfNeeded() {
  if (last_error == "http_post_failed" || last_error == "http_bad_status" ||
      last_error == "http_begin_failed" || last_error == "wifi_reconnect_failed") {
    last_error = "";
  }
}

void postIngestIfConfigured(const String& body) {
  if (strlen(kIngestUrl) == 0 || strlen(kWifiSsid) == 0) {
    return;
  }

  ensureWifiForPost();
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  HTTPClient http;
  http.setTimeout(kHttpPostTimeoutMs);
  if (!http.begin(kIngestUrl)) {
    last_error = "http_begin_failed";
    return;
  }

  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-OESIS-Parcel-Id", kParcelId);

  int code = http.POST(body);
  if (code == 202) {
    clearHttpLastErrorIfNeeded();
  } else if (code > 0) {
    last_error = "http_bad_status";
  } else {
    last_error = "http_post_failed";
  }
  http.end();
}
#else
void postIngestIfConfigured(const String& body) {
  (void)body;
}
#endif

void setup() {
  Serial.begin(115200);
  delay(250);
  Wire.begin(kI2cSdaPin, kI2cSclPin);

  Serial.println("# bench_air_node_serial_json");
  Serial.print("# I2C pin plan: SDA=GPIO");
  Serial.print(kI2cSdaPin);
  Serial.print(" SCL=GPIO");
  Serial.println(kI2cSclPin);
  Serial.println("# using Adafruit SHT4x and BME680 libraries");
  connectWifiAndSyncNtp();

  sht45_present = sht4.begin(&Wire);
  if (sht45_present) {
    sht4.setPrecision(SHT4X_HIGH_PRECISION);
    sht4.setHeater(SHT4X_NO_HEATER);
    Serial.println("# SHT45 detected");
  } else {
    last_error = "sht45_init_failed";
    Serial.println("# SHT45 not detected");
  }

  bme680_present = beginBme680();
  if (bme680_present) {
    Serial.print("# BME680 detected at 0x");
    if (bme680_address < 16) {
      Serial.print('0');
    }
    Serial.println(bme680_address, HEX);
    if (last_error == "sht45_init_failed") {
      // Keep the earlier init error if SHT45 is still missing.
    } else {
      last_error = "";
    }
  } else {
    last_error = "bme680_init_failed";
    Serial.println("# BME680 not detected");
  }
}

void loop() {
  unsigned long now = millis();
  if (now - last_sample_ms < kSampleIntervalMs) {
    delay(50);
    return;
  }

  last_sample_ms = now;

#if BENCH_AIR_HAS_WIFI
  wifi_connected = (WiFi.status() == WL_CONNECTED);
#else
  wifi_connected = false;
#endif

  Sht45Reading sht45 = readSht45();
  Bme680Reading bme680 = readBme680();
  updateObservedAt();

  if (!sht45.present || !bme680.present) {
    read_failures_total++;
  } else if (last_error == "sht45_read_failed" || last_error == "bme680_read_failed") {
    last_error = "";
  }

  String packet = buildPacketJson(sht45, bme680);
  emitPacketToSerial(packet);
  postIngestIfConfigured(packet);
}
