# Wiring

## Controller

- Recommended controller: ESP32-S3 dev board with exposed `3V3`, `GND`, `GPIO`, and native USB for programming.

## Sensors

- SHT45 for primary temperature and relative humidity
- BME680 for pressure and gas-trend support
- SPS30 for particulate sensing
- optional wind and rain sensors only after the PM-first build is stable

## Power rails

- `3V3` and `GND` from ESP32-S3 to the environmental sensors
- SPS30 powered according to its module documentation and isolated from bad enclosure airflow
- USB power into the ESP32-S3 only for first-build bring-up

## Pin map

Suggested first-build pin map:

| Function | ESP32-S3 DevKitC-1 | SHT45 | BME680 | SPS30 |
| --- | --- | --- | --- | --- |
| 3.3 V | `3V3` | `VIN` or `3V3` | `VIN` or `3V3` | module-specific |
| Ground | `GND` | `GND` | `GND` | `GND` |
| I2C SDA | `GPIO8` | `SDA` | `SDI` or `SDA` | n/a in first scaffold |
| I2C SCL | `GPIO9` | `SCL` | `SCK` or `SCL` | n/a in first scaffold |

This first scaffold keeps the environmental I2C bus on `GPIO8` and `GPIO9` and leaves PM integration packet-first rather than fully transport-specific.

## Notes on cable lengths and shielding

- Keep the SHT45 in clean shielded airflow.
- Keep the SPS30 airflow path clear and protected from splash.
- Keep controller heat away from both the SHT45 and SPS30 intake.
- Secure all mast wiring before field runs.

## Known risks

- Particulate sensors are more maintenance-sensitive than the simpler environmental stack.
- Outdoor mast vibration, dust, and water exposure can create false troubleshooting signals.
- Wind/rain additions will complicate both wiring and packet design and should not be mixed into the first PM-first proof unless necessary.
