# Serial JSON Contract

## Purpose

Define the exact serial payload shape the mast-lite node should emit during first-build bring-up so outdoor firmware output can be checked locally before any persistent transport exists.

## Transport rule

- emit one complete JSON object per line
- use UTF-8 text over the serial monitor
- keep the serial monitor at `115200`
- do not mix debug chatter into the same line as JSON

## Required packet shape

```json
{
  "schema_version": "oesis.bench-air.v1",
  "node_id": "mast-lite-01",
  "observed_at": "2026-03-30T19:45:00Z",
  "firmware_version": "0.1.0",
  "location_mode": "sheltered",
  "sensors": {
    "sht45": {
      "present": true,
      "temperature_c": 18.4,
      "relative_humidity_pct": 62.1
    },
    "bme680": {
      "present": true,
      "temperature_c": 19.1,
      "relative_humidity_pct": 60.8,
      "pressure_hpa": 1014.2,
      "gas_resistance_ohm": 152340
    }
  },
  "derived": {
    "temperature_c_primary": 18.4,
    "relative_humidity_pct_primary": 62.1,
    "pressure_hpa": 1014.2,
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

- keep `schema_version` at `oesis.bench-air.v1` for the first mast-lite build
- use `node_id` such as `mast-lite-01`
- use `location_mode` `sheltered` or `outdoor` based on the real placement
- keep the SHT45 as the primary temperature and humidity source
- keep the BME680 as pressure and gas-trend support

## What this contract adds relative to bench-air

This contract keeps the same packet lineage as `bench-air-node` but changes the
interpretation surface:

- it is intended for parcel-edge sheltered or outdoor reference conditions
- it improves outdoor heat and weather context for the parcel
- it still does **not** provide direct PM-based smoke evidence

That distinction matters for the later smoke-response bridge: `mast-lite` can be
the outdoor condition trigger, but the later closed loop still needs indoor PM,
equipment-state, and verification surfaces to say whether the house actually
protected occupants.

## Current firmware behavior

The first firmware scaffold supports:

- serial-only bring-up with placeholder `observed_at`
- optional Wi-Fi/NTP time sync for real UTC timestamps

## Local check path

1. Copy one emitted JSON line into `packet.json`
2. Run `python3 -m oesis.ingest.ingest_packet packet.json` from `oesis-runtime` repo root
3. Confirm the normalized observation prints successfully
