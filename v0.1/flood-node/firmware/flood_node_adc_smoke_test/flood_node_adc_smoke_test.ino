#include <Arduino.h>

static const int kAnalogPin = 4;
static const float kAdcReferenceVoltage = 3.3f;
static const int kAdcMaxCount = 4095;
static const float kVoltageDividerRatio = 2.0f;
static const unsigned long kSampleIntervalMs = 1000;

unsigned long last_sample_ms = 0;

float adcToBoardVoltage(int raw) {
  return (static_cast<float>(raw) / kAdcMaxCount) * kAdcReferenceVoltage;
}

float boardToSensorVoltage(float board_voltage) {
  return board_voltage * kVoltageDividerRatio;
}

void setup() {
  Serial.begin(115200);
  delay(250);
  analogReadResolution(12);
  Serial.println("# flood_node_adc_smoke_test");
  Serial.print("# analog pin: GPIO");
  Serial.println(kAnalogPin);
}

void loop() {
  unsigned long now = millis();
  if (now - last_sample_ms < kSampleIntervalMs) {
    delay(50);
    return;
  }
  last_sample_ms = now;

  int raw = analogRead(kAnalogPin);
  float board_voltage = adcToBoardVoltage(raw);
  float sensor_voltage = boardToSensorVoltage(board_voltage);

  Serial.print("analog_raw=");
  Serial.print(raw);
  Serial.print(" board_voltage_v=");
  Serial.print(board_voltage, 3);
  Serial.print(" sensor_voltage_v=");
  Serial.println(sensor_voltage, 3);
}
