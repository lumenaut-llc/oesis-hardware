// Utility sketch for reprogramming a PZEM module Modbus address.
// IMPORTANT: Connect only ONE PZEM module to the bus before running.
// Once the address is changed, power-cycle the module before connecting
// it to a shared bus with other modules.

#include <Arduino.h>
#include <PZEM004Tv30.h>

static const int kPzemRxPin = 16;
static const int kPzemTxPin = 17;

// Change this to the desired new address (0x01 - 0xF7).
static const uint8_t kNewAddress = 0x02;

PZEM004Tv30 pzem(Serial2, kPzemRxPin, kPzemTxPin);

void setup() {
  Serial.begin(115200);
  delay(250);
  Serial.println("# circuit_monitor_address_change");
  Serial.print("# Target new address: 0x0");
  Serial.println(kNewAddress, HEX);
  Serial.println();

  // Read current address
  uint8_t currentAddr = pzem.readAddress();
  if (currentAddr == 0) {
    Serial.println("ERROR: No PZEM module detected.");
    Serial.println("Check wiring and ensure only ONE module is connected.");
    return;
  }

  Serial.print("Current address: 0x0");
  Serial.println(currentAddr, HEX);

  if (currentAddr == kNewAddress) {
    Serial.println("Module is already at the target address. No change needed.");
    return;
  }

  // Set new address
  bool success = pzem.setAddress(kNewAddress);

  if (success) {
    Serial.print("Address changed to: 0x0");
    Serial.println(kNewAddress, HEX);
    Serial.println("Power-cycle the module before connecting to a shared bus.");
  } else {
    Serial.println("ERROR: Address change failed.");
    Serial.println("Try power-cycling the module and running again.");
  }
}

void loop() {
  // Nothing to do after address change.
  delay(10000);
}
