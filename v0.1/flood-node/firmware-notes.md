# Firmware Notes

## Responsibilities

The first flood-node firmware should:
- read the protected MB7389 analog signal
- convert raw ADC to a provisional distance estimate
- derive provisional water depth from a dry-reference constant
- estimate rise rate between samples
- emit one JSON packet per line for local ingest testing
- report health and calibration state clearly

It should stay narrowly focused on low-point runoff evidence. It should not try
to infer parcel-wide flood truth, route status, or equipment-state from the node
itself.

## Sampling cadence

- default sample interval: every `5` seconds
- keep the first build slow and readable
- prefer stable, low-drama telemetry over aggressive filtering or event logic

## Packet shape

Use the `oesis.flood-node.v1` packet contract in `serial-json-contract.md`.

Key points:
- include `schema_version`, `node_id`, `observed_at`, and `firmware_version`
- expose both raw ADC and inferred distance
- include `water_depth_cm` and `rise_rate_cm_per_hr`
- include a `calibration_state` or equivalent flag so provisional numbers are obvious

## Error handling

- emit packets even when calibration is provisional
- keep `last_error` populated when signal reads fail or Wi-Fi/NTP setup fails
- increment `read_failures_total` when the analog read path looks invalid
- prefer explicit degraded-state packets over silent output gaps

## Health telemetry

Minimum health fields:
- `uptime_s`
- `wifi_connected`
- `free_heap_bytes`
- `read_failures_total`
- `last_error`

Recommended flood-specific health context:
- ADC raw value
- inferred sensor voltage
- calibration state
- dry-reference constant in use

## Future enhancements

- move from provisional analog scaling to finalized on-site calibration
- evaluate median filtering or outlier rejection after field logs exist
- add stronger event annotations for rapid rise conditions
- tie low-point install metadata to parcel context more directly
