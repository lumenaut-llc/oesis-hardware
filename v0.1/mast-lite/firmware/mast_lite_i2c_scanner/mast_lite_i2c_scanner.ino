#include <Arduino.h>
#include <Wire.h>

static const int kI2cSdaPin = 8;
static const int kI2cSclPin = 9;
static const unsigned long kScanIntervalMs = 5000;

unsigned long last_scan_ms = 0;

void scanI2cBus() {
  int devices_found = 0;
  Serial.println("I2C scan starting...");
  for (uint8_t address = 1; address < 127; ++address) {
    Wire.beginTransmission(address);
    uint8_t error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("Found device at 0x");
      if (address < 16) {
        Serial.print('0');
      }
      Serial.println(address, HEX);
      devices_found++;
    }
  }
  Serial.print("Total devices found: ");
  Serial.println(devices_found);
}

void setup() {
  Serial.begin(115200);
  delay(250);
  Wire.begin(kI2cSdaPin, kI2cSclPin);
  Serial.println("# mast_lite_i2c_scanner");
  Serial.print("# I2C pin plan: SDA=GPIO");
  Serial.print(kI2cSdaPin);
  Serial.print(" SCL=GPIO");
  Serial.println(kI2cSclPin);
}

void loop() {
  unsigned long now = millis();
  if (now - last_scan_ms < kScanIntervalMs) {
    delay(50);
    return;
  }
  last_scan_ms = now;
  scanI2cBus();
}
