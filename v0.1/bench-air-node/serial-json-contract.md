# Serial JSON Contract

## Purpose

Define the exact serial payload shape the bench air node should emit during first-build bring-up so firmware output can be checked locally before any Wi-Fi or HTTP transport exists.

## Transport rule

- emit one complete JSON object per line
- use UTF-8 text over the serial monitor
- keep the serial monitor at `115200`
- do not mix debug chatter into the same line as JSON

During first-build bring-up it is fine to print a few human-readable boot messages, but every packet line should be valid standalone JSON.

## Required packet shape

```json
{
  "schema_version": "oesis.bench-air.v1",
  "node_id": "bench-air-01",
  "observed_at": "2026-03-30T19:45:00Z",
  "firmware_version": "0.1.0",
  "location_mode": "indoor",
  "sensors": {
    "sht45": {
      "present": true,
      "temperature_c": 23.4,
      "relative_humidity_pct": 41.8
    },
    "bme680": {
      "present": true,
      "temperature_c": 24.1,
      "relative_humidity_pct": 40.9,
      "pressure_hpa": 1012.6,
      "gas_resistance_ohm": 145230
    }
  },
  "derived": {
    "temperature_c_primary": 23.4,
    "relative_humidity_pct_primary": 41.8,
    "pressure_hpa": 1012.6,
    "voc_trend_source": "gas_resistance_ohm"
  },
  "health": {
    "uptime_s": 1820,
    "wifi_connected": false,
    "free_heap_bytes": 214332,
    "read_failures_total": 0,
    "last_error": null
  }
}
```

## Field guidance

- `schema_version`: keep fixed at `oesis.bench-air.v1` for the MVP
- `node_id`: stable per device, such as `bench-air-01`
- `observed_at`: use an RFC 3339 UTC timestamp when real time is available
- `firmware_version`: match the flashed firmware build
- `location_mode`: use `indoor` for the first bench build
- `sht45`: primary temperature and humidity source
- `bme680`: pressure and gas-trend source, plus secondary temp and humidity
- `wifi_connected`: `false` is expected for serial-only bring-up

## What this contract can and cannot mean

This contract is the baseline indoor or sheltered evidence contract.

It is appropriate for:

- indoor temperature and humidity evidence
- pressure context
- gas-trend anomaly detection
- device-health and packet-timing validation

It is **not** sufficient by itself for:

- direct PM2.5 measurement
- direct smoke concentration claims
- house-response verification

The later `v1.5` bridge will need additional packet families or support objects
for indoor PM2.5, equipment state, actions, and measured outcomes.

If Wi-Fi credentials are configured for time sync, `wifi_connected` may be `true` even when packets are still only being inspected over serial.

## First-build fallback for time

If the node does not yet have a real clock, use one of these approaches consistently:

- emit boot-relative timestamps only in debug logs and wait to emit JSON packets until time is available
- emit a placeholder RFC 3339 timestamp and clearly document that it is provisional during serial-only bring-up

The preferred path for the MVP is to emit valid RFC 3339 UTC timestamps once Wi-Fi/NTP or another clock source exists.

## Current firmware behavior

The current bench-air firmware scaffold supports two modes:

- serial-only bring-up: blank Wi-Fi credentials, placeholder `observed_at`, `wifi_connected: false`
- Wi-Fi-assisted time sync: configured credentials, NTP-backed UTC `observed_at`, `wifi_connected: true` when connected

## Serial line example

This is the shape the serial monitor should show on a single line:

```json
{"schema_version":"oesis.bench-air.v1","node_id":"bench-air-01","observed_at":"2026-03-30T19:45:00Z","firmware_version":"0.1.0","location_mode":"indoor","sensors":{"sht45":{"present":true,"temperature_c":23.4,"relative_humidity_pct":41.8},"bme680":{"present":true,"temperature_c":24.1,"relative_humidity_pct":40.9,"pressure_hpa":1012.6,"gas_resistance_ohm":145230}},"derived":{"temperature_c_primary":23.4,"relative_humidity_pct_primary":41.8,"pressure_hpa":1012.6,"voc_trend_source":"gas_resistance_ohm"},"health":{"uptime_s":1820,"wifi_connected":false,"free_heap_bytes":214332,"read_failures_total":0,"last_error":null}}
```

## Local check path

1. Copy one emitted JSON line into a file such as `packet.json`
2. Run `python3 -m oesis.ingest.ingest_packet packet.json` from `oesis-runtime` repo root
3. Confirm the normalized observation prints successfully

If that command succeeds, the packet shape is good enough for the local ingest scaffold.
