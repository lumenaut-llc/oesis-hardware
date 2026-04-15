#include <Arduino.h>
#include <Wire.h>
#include <time.h>
#include <Adafruit_BME680.h>
#include <Adafruit_SHT4x.h>
#include <Adafruit_Sensor.h>

#if __has_include(<WiFi.h>)
#include <WiFi.h>
#define WEATHER_PM_MAST_HAS_WIFI 1
#else
#define WEATHER_PM_MAST_HAS_WIFI 0
#endif

#if __has_include("secrets.h")
#include "secrets.h"
#endif

#ifndef WEATHER_PM_MAST_WIFI_SSID
#define WEATHER_PM_MAST_WIFI_SSID ""
#endif

#ifndef WEATHER_PM_MAST_WIFI_PASSWORD
#define WEATHER_PM_MAST_WIFI_PASSWORD ""
#endif

static const char* kSchemaVersion = "oesis.weather-pm-mast.v1";
static const char* kNodeId = "weather-pm-mast-01";
static const char* kFirmwareVersion = "0.1.0";
static const char* kLocationMode = "outdoor";
static const char* kObservedAtPlaceholder = "1970-01-01T00:00:00Z";
static const char* kWifiSsid = WEATHER_PM_MAST_WIFI_SSID;
static const char* kWifiPassword = WEATHER_PM_MAST_WIFI_PASSWORD;
static const char* kNtpServer = "pool.ntp.org";
static const long kGmtOffsetSeconds = 0;
static const int kDaylightOffsetSeconds = 0;
static const int kI2cSdaPin = 8;
static const int kI2cSclPin = 9;
static const unsigned long kSampleIntervalMs = 5000;
static const unsigned long kWifiConnectTimeoutMs = 15000;
static const unsigned long kTimeSyncTimeoutMs = 15000;
static const uint8_t kBme680Addresses[] = {0x77, 0x76};

struct Sht45Reading { bool present; float temperature_c; float relative_humidity_pct; };
struct Bme680Reading { bool present; float temperature_c; float relative_humidity_pct; float pressure_hpa; float gas_resistance_ohm; };

unsigned long last_sample_ms = 0;
unsigned long read_failures_total = 0;
bool sht45_present = false;
bool bme680_present = false;
String last_error = "boot";
bool wifi_connected = false;
bool time_synced = false;
String observed_at = kObservedAtPlaceholder;

Adafruit_SHT4x sht4 = Adafruit_SHT4x();
Adafruit_BME680 bme680;

void printFloat(float value, int digits = 1) { Serial.print(value, digits); }

void printOptionalFloat(bool has_value, float value, int digits = 1) {
  if (!has_value) {
    Serial.print("null");
    return;
  }
  printFloat(value, digits);
}

void printJsonString(const String& value) {
  Serial.print('"');
  for (size_t i = 0; i < value.length(); ++i) {
    char c = value.charAt(i);
    if (c == '"' || c == '\\') { Serial.print('\\'); }
    Serial.print(c);
  }
  Serial.print('"');
}

void updateObservedAt() {
  if (!time_synced) { observed_at = kObservedAtPlaceholder; return; }
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

Sht45Reading readSht45() {
  Sht45Reading reading = {false, 0.0f, 0.0f};
  if (!sht45_present) return reading;
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
  Bme680Reading reading = {false, 0.0f, 0.0f, 0.0f, 0.0f};
  if (!bme680_present) return reading;
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

bool beginBme680() {
  for (size_t i = 0; i < sizeof(kBme680Addresses) / sizeof(kBme680Addresses[0]); ++i) {
    if (bme680.begin(kBme680Addresses[i], &Wire)) {
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

void syncTimeIfConfigured() {
  if (strlen(kWifiSsid) == 0) return;
#if !WEATHER_PM_MAST_HAS_WIFI
  last_error = "wifi_unavailable";
  return;
#else
  WiFi.mode(WIFI_STA);
  WiFi.begin(kWifiSsid, kWifiPassword);
  unsigned long wifi_start_ms = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - wifi_start_ms < kWifiConnectTimeoutMs) delay(250);
  wifi_connected = WiFi.status() == WL_CONNECTED;
  if (!wifi_connected) { last_error = "wifi_connect_failed"; return; }
  configTime(kGmtOffsetSeconds, kDaylightOffsetSeconds, kNtpServer);
  unsigned long time_start_ms = millis();
  while (!time_synced && millis() - time_start_ms < kTimeSyncTimeoutMs) {
    struct tm time_info;
    if (getLocalTime(&time_info, 250)) { time_synced = true; break; }
  }
  if (!time_synced) { last_error = "ntp_sync_failed"; return; }
  updateObservedAt();
#endif
}

void emitPacket(const Sht45Reading& sht45, const Bme680Reading& bme680_reading) {
  float derived_temperature_c = sht45.present ? sht45.temperature_c : bme680_reading.temperature_c;
  float derived_relative_humidity_pct = sht45.present ? sht45.relative_humidity_pct : bme680_reading.relative_humidity_pct;
  bool has_primary_temperature = sht45.present || bme680_reading.present;
  bool has_primary_humidity = sht45.present || bme680_reading.present;

  Serial.print("{\"schema_version\":\""); Serial.print(kSchemaVersion);
  Serial.print("\",\"node_id\":\""); Serial.print(kNodeId);
  Serial.print("\",\"observed_at\":\""); Serial.print(observed_at);
  Serial.print("\",\"firmware_version\":\""); Serial.print(kFirmwareVersion);
  Serial.print("\",\"location_mode\":\""); Serial.print(kLocationMode);
  Serial.print("\",\"sensors\":{\"sht45\":{\"present\":"); Serial.print(sht45.present ? "true" : "false");
  Serial.print(",\"temperature_c\":"); printFloat(sht45.temperature_c);
  Serial.print(",\"relative_humidity_pct\":"); printFloat(sht45.relative_humidity_pct);
  Serial.print("},\"bme680\":{\"present\":"); Serial.print(bme680_reading.present ? "true" : "false");
  Serial.print(",\"temperature_c\":"); printFloat(bme680_reading.temperature_c);
  Serial.print(",\"relative_humidity_pct\":"); printFloat(bme680_reading.relative_humidity_pct);
  Serial.print(",\"pressure_hpa\":"); printFloat(bme680_reading.pressure_hpa);
  Serial.print(",\"gas_resistance_ohm\":"); printFloat(bme680_reading.gas_resistance_ohm, 0);
  Serial.print("},\"sps30\":{\"present\":false}},\"derived\":{\"temperature_c_primary\":");
  printOptionalFloat(has_primary_temperature, derived_temperature_c);
  Serial.print(",\"relative_humidity_pct_primary\":"); printOptionalFloat(has_primary_humidity, derived_relative_humidity_pct);
  Serial.print(",\"pressure_hpa\":"); printOptionalFloat(bme680_reading.present, bme680_reading.pressure_hpa);
  Serial.print(",\"voc_trend_source\":");
  if (bme680_reading.present) {
    Serial.print("\"gas_resistance_ohm\"");
  } else {
    Serial.print("null");
  }
  Serial.print("},\"health\":{\"uptime_s\":");
  Serial.print(millis() / 1000UL);
  Serial.print(",\"wifi_connected\":"); Serial.print(wifi_connected ? "true" : "false");
  Serial.print(",\"free_heap_bytes\":");
#ifdef ESP32
  Serial.print(ESP.getFreeHeap());
#else
  Serial.print(0);
#endif
  Serial.print(",\"read_failures_total\":"); Serial.print(read_failures_total);
  Serial.print(",\"last_error\":");
  if (last_error.length() == 0) Serial.print("null"); else printJsonString(last_error);
  Serial.print("}}");
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  delay(250);
  Wire.begin(kI2cSdaPin, kI2cSclPin);
  Serial.println("# weather_pm_mast_serial_json");
  syncTimeIfConfigured();
  sht45_present = sht4.begin(&Wire);
  if (sht45_present) {
    sht4.setPrecision(SHT4X_HIGH_PRECISION);
    sht4.setHeater(SHT4X_NO_HEATER);
  } else {
    last_error = "sht45_init_failed";
  }
  bme680_present = beginBme680();
  if (!bme680_present) last_error = "bme680_init_failed";
  else if (last_error != "sht45_init_failed") last_error = "";
}

void loop() {
  unsigned long now = millis();
  if (now - last_sample_ms < kSampleIntervalMs) { delay(50); return; }
  last_sample_ms = now;
  Sht45Reading sht45 = readSht45();
  Bme680Reading bme680_reading = readBme680();
  updateObservedAt();
  if (!sht45.present || !bme680_reading.present) read_failures_total++;
  emitPacket(sht45, bme680_reading);
}
