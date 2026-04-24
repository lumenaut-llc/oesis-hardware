# Firmware Notes

## Responsibilities

- boot reliably and identify installed sensors
- sample the environmental stack on a predictable cadence and only emit PM data after real SPS30 integration
- emit timestamped JSON packets with node metadata
- stay backward-compatible with the simpler ingest path where practical
- surface health telemetry and read failures without hiding them

This firmware should justify its extra complexity with genuinely richer outdoor
PM and weather evidence. It should not exist merely as a fancier replacement for
`mast-lite` when the simpler lane is still sufficient.

## Sampling cadence

- default PM-first mast cadence: every 15 seconds
- quiet long-run mode: every 60 seconds
- warm-up period: discard or flag the first 2 to 5 samples after boot

## Packet shape

Suggested first-build packet:

```json
{
  "schema_version": "oesis.weather-pm-mast.v1",
  "node_id": "weather-pm-mast-01",
  "observed_at": "2026-03-30T19:45:00Z",
  "firmware_version": "0.1.0",
  "location_mode": "outdoor",
  "sensors": {
    "sht45": {
      "present": true,
      "temperature_c": 17.9,
      "relative_humidity_pct": 66.2
    },
    "bme680": {
      "present": true,
      "temperature_c": 18.6,
      "relative_humidity_pct": 64.4,
      "pressure_hpa": 1013.9,
      "gas_resistance_ohm": 149880
    },
    "sps30": {
      "present": false
    }
  },
  "derived": {
    "temperature_c_primary": 17.9,
    "relative_humidity_pct_primary": 66.2,
    "pressure_hpa": 1013.9,
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

The exact first-build line-oriented payload target is documented in `serial-json-contract.md`.

## Error handling

- if one sensor fails, continue publishing the remaining sensor data with explicit presence and error fields
- during first-build bring-up, fail fast in scanner and serial test sketches rather than hiding missing devices
- count cumulative read failures
- keep wind/rain sensors out of packets until they are genuinely integrated

## Health telemetry

- uptime
- free heap or equivalent memory watermark
- Wi-Fi connection state if time sync is enabled
- sensor presence at boot
- cumulative read failures
- packet sequence counter

## Future enhancements

- real SPS30 transport integration
- wind and rain channels
- richer public smoke and local PM comparison logic
- maintenance counters for particulate service intervals
