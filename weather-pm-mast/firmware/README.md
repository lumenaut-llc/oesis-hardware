# Firmware Scaffold

## Purpose

Provide a minimal ESP32 bring-up sketch set for the weather-pm-mast so the PM-first outdoor node can be validated before full wind and rain mechanics are added.

## Staging role

This firmware is for the richer second-wave outdoor lane, not the first-kit
baseline. Use it after `mast-lite` is already stable and there is a real reason
to add PM-driven outdoor smoke evidence and more detailed exposed weather
mechanics.

## Current contents

- `weather_pm_mast_i2c_scanner/weather_pm_mast_i2c_scanner.ino`
- `weather_pm_mast_serial_json/weather_pm_mast_serial_json.ino`
- `weather_pm_mast_serial_json/secrets.example.h`
- `platformio.ini`
- `tools/capture_serial.sh`

## Build setup

```bash
pio run -e weather_pm_mast_i2c_scanner
pio run -e weather_pm_mast_serial_json
```

## Notes

The first scaffold keeps the `sps30` section present but marked unavailable until a real SPS30 transport integration is added. The purpose is to lock down the packet path, mast workflow, and build system first without fabricating live PM values.
