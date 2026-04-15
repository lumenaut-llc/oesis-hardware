# Operator Runbook

## Purpose

Provide the shortest repeatable path from freshly wired bench hardware to a locally validated normalized observation.

## What this runbook proves

This runbook proves the minimum indoor evidence path for the parcel-first
system:

- the bench node emits a valid `oesis.bench-air.v1` packet
- the local ingest path can normalize that packet
- one parcel can receive indoor or sheltered evidence from a real device

It does **not** prove the later bridge surfaces yet:

- indoor PM2.5 response sensing
- power / outage state
- equipment-state capture
- action logging
- outcome verification

## Before you start

- ESP32-S3 DevKitC-1 wired with `GPIO8` as `SDA` and `GPIO9` as `SCL`
- SHT45 connected first
- BME680 ready to add after the first scan passes
- Arduino IDE or equivalent ESP32 workflow installed
- sibling `oesis-runtime` checkout installed (`pip install -e .`) so `python3 -m oesis.ingest.ingest_packet` works

## Arduino libraries

Install these libraries before flashing the serial JSON sketch:

- `Adafruit SHT4x Library`
- `Adafruit BME680 Library`
- `Adafruit Unified Sensor`

## Stage 1: scan SHT45 only

1. Flash `firmware/bench_air_node_i2c_scanner/bench_air_node_i2c_scanner.ino`
2. Open the serial monitor at `115200`
3. Confirm you see `Found device at 0x44`

Expected output shape:

```text
I2C scan starting...
Found device at 0x44
Total devices found: 1
```

Stop here if `0x44` does not appear.

## Stage 2: add BME680 and scan again

1. Power off the board
2. Add the BME680 to the same `3V3`, `GND`, `SDA`, and `SCL` bus
3. Power back on with the same scanner sketch
4. Confirm you see `0x44` plus `0x76` or `0x77`

Expected output shape:

```text
I2C scan starting...
Found device at 0x44
Found device at 0x77
Total devices found: 2
```

Stop here if the second address does not appear.

## Stage 3: flash serial JSON sketch

1. Flash `firmware/bench_air_node_serial_json/bench_air_node_serial_json.ino`
2. Open the serial monitor at `115200`
3. Ignore the boot comment lines that start with `#`
4. Copy one full JSON packet line into a file named `packet.json`

Optional before flashing:

- copy `firmware/bench_air_node_serial_json/secrets.example.h` to `firmware/bench_air_node_serial_json/secrets.h`
- fill in your Wi-Fi credentials there if you want real UTC timestamps via NTP
- optional: set `BENCH_AIR_INGEST_URL` to your ingest service (see **Optional HTTP ingest over Wi‑Fi** below) and `BENCH_AIR_PARCEL_ID` to match your parcel context

Expected boot comments:

```text
# bench_air_node_serial_json
# I2C pin plan: SDA=GPIO8 SCL=GPIO9
```

Expected packet behavior:

- one JSON object per line
- one packet about every 5 seconds
- `sht45` and `bme680` both present when wiring is healthy
- `observed_at` is a real UTC timestamp only if Wi-Fi time sync is configured and succeeds

## Stage 4: validate locally

From the `oesis-runtime` repository root (after `pip install -e .`):

```bash
python3 -m oesis.ingest.ingest_packet packet.json --parcel-id parcel_demo_001
```

If you prefer stdin:

```bash
python3 -m oesis.ingest.ingest_packet - --parcel-id parcel_demo_001 < packet.json
```

If you captured a whole serial monitor log with `#` boot lines, extract the newest packet first:

```bash
python3 -m oesis.ingest.extract_latest_packet serial.log --output packet.json
python3 -m oesis.ingest.ingest_packet packet.json --parcel-id parcel_demo_001
```

Recommended macOS capture flow:

```bash
cd ../bench-air-node/firmware
./tools/capture_serial.sh /dev/cu.usbmodem101 serial.log
```

Continuous forward (USB serial → reference ingest API, no log file): from `oesis-runtime` root, `pip install -e ".[serial-bridge]"`, start `python3 -m oesis.ingest.serve_ingest_api`, then run `python3 -m oesis.ingest.serial_bridge --serial-port /dev/cu.usbmodem101 --parcel-id parcel_demo_001` (see sibling repo `README.md`).

### Optional HTTP ingest over Wi‑Fi (no USB bridge)

The serial JSON sketch can **POST each packet** to the reference ingest API when Wi‑Fi credentials and ingest URL are set in `secrets.h`:

1. On a machine reachable from the ESP32 (same LAN), run ingest bound to all interfaces, e.g. from `oesis-runtime` root:  
   `python3 -m oesis.ingest.serve_ingest_api --host 0.0.0.0 --port 8787`
2. In `secrets.h`, set `BENCH_AIR_WIFI_SSID` / `BENCH_AIR_WIFI_PASSWORD`, `BENCH_AIR_INGEST_URL` to `http://<that-machine-LAN-IP>:8787/v1/ingest/node-packets`, and `BENCH_AIR_PARCEL_ID` to the parcel id you use downstream (must match `X-OESIS-Parcel-Id` expectations).
3. Flash and open the serial monitor: you should see `# HTTP ingest enabled for POST /v1/ingest/node-packets` after a successful Wi‑Fi connect, plus one JSON line per interval as before.

Notes:

- **Plain HTTP on LAN only** in this reference firmware; HTTPS/TLS is not implemented here.
- If `BENCH_AIR_INGEST_URL` is set but Wi‑Fi SSID is empty, the sketch disables POST and prints a `#` comment explaining why.
- Ensure the host firewall allows TCP `8787` from the device. `health.last_error` may show `http_post_failed`, `http_bad_status`, or `wifi_reconnect_failed` when uploads fail.
- To confirm the service is receiving packets in a browser, open **`http://<ingest-host>:8787/v1/ingest/live`** on the ingest machine (same URL pattern if you use another port).

## Pass criteria

- scanner finds `0x44` first
- scanner finds `0x44` plus `0x76` or `0x77` after BME680 is added
- serial JSON sketch prints one valid packet line every 5 seconds
- `python3 -m oesis.ingest.ingest_packet packet.json --parcel-id parcel_demo_001` succeeds
- normalized output includes `temperature_c_primary`, `relative_humidity_pct_primary`, `pressure_hpa`, and `gas_resistance_ohm`

## Known first-build limitations

- `observed_at` remains a placeholder if Wi-Fi time sync is not configured or fails
- Wi-Fi is used for optional NTP and, when `BENCH_AIR_INGEST_URL` is set, optional **HTTP POST** to ingest (still no TLS in this sketch)
- gas resistance is useful for trend inspection, not calibrated AQI

## If something fails

- no `0x44`: recheck SHT45 power, solder, and `GPIO8`/`GPIO9` wiring
- no BME680 address: recheck `SDI` to `SDA`, `SCK` to `SCL`, and power
- JSON does not validate: copy a full packet line, not a boot comment
- mixed serial log is messy: run `python3 -m oesis.ingest.extract_latest_packet serial.log --output packet.json`
- only one sensor reports `present: false`: inspect that sensor’s solder joints and bus wiring
