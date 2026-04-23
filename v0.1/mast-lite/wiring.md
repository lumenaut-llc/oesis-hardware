# Wiring

## Controller

- Recommended controller: ESP32-S3 dev board with exposed `3V3`, `GND`, `GPIO`, and native USB for programming.

## Sensors

- SHT45 for primary temperature and relative humidity
- BME680 for secondary temperature and humidity, plus pressure and gas-trend evidence
- optional UV sensor only after the base build is stable

## Power rails

- `3V3` from ESP32-S3 to both sensor boards
- common `GND` to both sensor boards
- USB power into the ESP32-S3 only for first-build bring-up

## Pin map

Suggested default pin map:

| Function | ESP32-S3 DevKitC-1 | SHT45 | BME680 |
| --- | --- | --- | --- |
| 3.3 V | `3V3` | `VIN` or `3V3` | `VIN` or `3V3` |
| Ground | `GND` | `GND` | `GND` |
| I2C SDA | `GPIO8` | `SDA` | `SDI` or `SDA` |
| I2C SCL | `GPIO9` | `SCL` | `SCK` or `SCL` |

This project's default mast-lite wiring uses `GPIO8` for `SDA` and `GPIO9` for `SCL`.

## Notes on cable lengths and shielding

- Keep the controller and BME680 inside or near the enclosure, but keep the SHT45 positioned for cleaner airflow.
- Keep the SHT45 away from heat-trapping enclosure surfaces.
- Secure and strain-relieve all wires before outdoor mounting.
- Use a radiation shield or similar shade/vent structure for the SHT45.
- Keep cable runs as short as practical during the first outdoor build.

## Known risks

- Outdoor temperature bias is more likely to come from siting than from sensor error.
- The BME680 self-heats slightly and should not be used as the primary outdoor temperature reference.
- Long or poorly supported I2C runs can create intermittent outdoor faults that mimic firmware bugs.
- Water ingress and condensation are more immediate risks than software instability in this subsystem.
