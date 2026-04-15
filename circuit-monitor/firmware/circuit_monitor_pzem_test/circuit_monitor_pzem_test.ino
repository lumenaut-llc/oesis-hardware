// Circuit monitor PZEM Modbus communication test for ESP32-S3.
// Reads voltage, current, power, energy, frequency, and power factor
// from one or two PZEM modules on UART2 (GPIO16 RX, GPIO17 TX).
// Prints human-readable output to Serial0 at 115200.

#include <Arduino.h>
#include <PZEM004Tv30.h>

static const int kPzemRxPin = 16;  // ESP32 RX <- PZEM TX
static const int kPzemTxPin = 17;  // ESP32 TX -> PZEM RX
static const uint8_t kAddress1 = 0x01;
static const uint8_t kAddress2 = 0x02;
static const unsigned long kReadIntervalMs = 5000;
static const bool kTwoModules = true;  // set false for single-module test

PZEM004Tv30 pzem1(Serial2, kPzemRxPin, kPzemTxPin, kAddress1);
PZEM004Tv30 pzem2(Serial2, kPzemRxPin, kPzemTxPin, kAddress2);

void printReading(const char* label, uint8_t address, PZEM004Tv30& pzem) {
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();
  float frequency = pzem.frequency();
  float pf = pzem.pf();

  Serial.print("PZEM address 0x0");
  Serial.print(address);
  Serial.println(":");

  if (isnan(voltage)) {
    Serial.println("  ERROR: no response (check wiring and address)");
    return;
  }

  Serial.print("  voltage: "); Serial.print(voltage, 1); Serial.println(" V");
  Serial.print("  current: "); Serial.print(current, 2); Serial.println(" A");
  Serial.print("  power: "); Serial.print(power, 1); Serial.println(" W");
  Serial.print("  energy: "); Serial.print(energy, 2); Serial.println(" kWh");
  Serial.print("  frequency: "); Serial.print(frequency, 1); Serial.println(" Hz");
  Serial.print("  power_factor: "); Serial.print(pf, 2); Serial.println();
}

void setup() {
  Serial.begin(115200);
  delay(250);
  Serial.println("# circuit_monitor_pzem_test");
  Serial.print("# UART2 pin plan: RX=GPIO"); Serial.print(kPzemRxPin);
  Serial.print(" TX=GPIO"); Serial.println(kPzemTxPin);
  Serial.println();
}

void loop() {
  printReading("primary", kAddress1, pzem1);
  Serial.println();

  if (kTwoModules) {
    printReading("secondary", kAddress2, pzem2);
    Serial.println();
  }

  Serial.println("---");
  delay(kReadIntervalMs);
}
