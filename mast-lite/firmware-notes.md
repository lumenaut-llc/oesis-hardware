# Firmware Notes

## Responsibilities

- boot reliably and identify installed sensors
- sample SHT45 and BME680 on a predictable cadence
- emit timestamped JSON packets with node metadata
- preserve continuity with the bench-air ingest contract
- surface health telemetry and read failures without hiding them

This firmware should remain the simple outdoor reference lane. It should not try
to absorb richer PM mechanics from `weather-pm-mast` or later `v1.5` bridge
objects such as indoor-response sensing, equipment-state, or verification.

## Sampling cadence

- default sheltered outdoor cadence: every 15 seconds
- quiet long-run mode: every 60 seconds
- warm-up period: discard or flag the first 2 to 5 samples after boot

Bring-up firmware should use `GPIO8` for `SDA` and `GPIO9` for `SCL`. The SHT45 remains the primary temperature and humidity source; the BME680 primarily contributes pressure and gas-trend evidence.

## Packet shape

Use the same `oesis.bench-air.v1` packet shape for the first mast-lite bring-up, with a different `node_id` and an outdoor-appropriate `location_mode`.

Suggested first-build packet:

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

The exact line-oriented payload target is documented in `serial-json-contract.md`.

## Error handling

- if one sensor fails, continue publishing the remaining sensor data with explicit presence and error fields
- during first-build bring-up, fail fast in scanner and bench-test sketches rather than hiding missing devices
- count consecutive read failures per sensor
- never silently substitute stale values for fresh reads
- surface Wi-Fi time-sync failure separately from sensor failure

## Health telemetry

- uptime
- free heap or equivalent memory watermark
- Wi-Fi connection state if time sync is enabled
- sensor presence at boot
- cumulative read failures
- packet sequence counter

## Future enhancements

- UV integration once the base environmental stack is stable
- local ring buffer for intermittent outdoor connectivity
- enclosure temperature compensation notes if needed
- eventual promotion into weather-pm-mast packet extensions

The later smoke closed loop should build around this firmware as the outdoor
condition trigger, then add separate indoor-response, equipment-state, action,
and verification surfaces rather than overloading the `mast-lite` packet.
