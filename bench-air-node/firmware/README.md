# Firmware Scaffold

## Purpose

Provide a minimal ESP32 bring-up sketch that emits the agreed `oesis.bench-air.v1` packet shape over serial before sensor-library integration and network transport are added.

## Staging role

This firmware is the smallest current-truth bring-up path for the parcel-sensing
baseline. It is meant to prove the indoor evidence lineage for **program-phase
`v0.1`**.

It is not yet the firmware for the later bridge surfaces such as:

- indoor PM2.5 response sensing
- power / outage state
- equipment-state capture
- action or verification records

## Current contents

- `bench_air_node_i2c_scanner/bench_air_node_i2c_scanner.ino`
  Minimal I2C scanner for `GPIO8` and `GPIO9`
- `bench_air_node_serial_json/bench_air_node_serial_json.ino`
  Arduino-style sketch for ESP32-S3 that reads SHT45 and BME680, then prints one JSON packet per line at `115200`
- `bench_air_node_serial_json/secrets.example.h`
  Copy-to-local template for optional Wi-Fi time-sync credentials
- `platformio.ini`
  Pinned ESP32-S3 build scaffold with separate environments for the scanner and serial JSON sketch
- `tools/capture_serial.sh`
  Simple macOS serial capture helper that writes a monitor log to disk

## Pinned build setup

The firmware directory now includes `platformio.ini` with a pinned ESP32 platform and library set:

- platform: `espressif32@6.7.0`
- board: `esp32-s3-devkitc-1`
- framework: Arduino

Recommended commands from this directory:

```bash
pio run -e bench_air_node_i2c_scanner
pio run -e bench_air_node_serial_json
```

## Arduino libraries

Install these libraries before compiling the serial JSON sketch:

- `Adafruit SHT4x Library`
- `Adafruit BME680 Library`
- `Adafruit Unified Sensor`

## Optional time sync

The serial JSON sketch can emit a real UTC `observed_at` timestamp if you fill in:

- `kWifiSsid`
- `kWifiPassword`

Recommended local pattern:

1. Copy `bench_air_node_serial_json/secrets.example.h` to `bench_air_node_serial_json/secrets.h`
2. Fill in your Wi-Fi credentials locally
3. Do not commit `secrets.h`

If credentials remain blank, the sketch stays in serial-only bring-up mode and uses the placeholder timestamp.

## First run

1. Open `bench_air_node_i2c_scanner.ino` in the Arduino IDE or another ESP32-capable workflow
2. Select the ESP32-S3 board
3. Confirm the sketch still uses `GPIO8` for `SDA` and `GPIO9` for `SCL`
4. Flash the scanner sketch and open the serial monitor at `115200`
5. Confirm the SHT45 appears at `0x44`
6. Add the BME680 and confirm it appears at `0x76` or `0x77`
7. Open `bench_air_node_serial_json.ino`
8. Flash the serial JSON sketch
9. Confirm you see one valid JSON object per line every 5 seconds
10. If Wi-Fi credentials are configured, confirm you see a `# time synced:` boot line and non-placeholder `observed_at` values

## Serial capture on macOS

To save a serial session to `serial.log`, first identify the device:

```bash
ls /dev/cu.usbmodem*
```

Then run:

```bash
./tools/capture_serial.sh /dev/cu.usbmodem101 serial.log
```

## Current limitations

- `observed_at` remains a placeholder unless Wi-Fi time sync is configured and succeeds
- Wi-Fi transport is not enabled

The immediate goal is to prove sensor bring-up, packet shape, and ingest compatibility first. Time sync and network transport can come next.

For the full bench workflow from flash to local ingest validation, use `../operator-runbook.md`.
