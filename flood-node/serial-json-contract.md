# Flood Node Serial JSON Contract

## Purpose

This document defines the first serial JSON payload for the low-point flood node.

The contract is intentionally modest:
- one JSON object per line
- readable over USB serial at `115200`
- stable enough for the local ingest scripts
- explicit about provisional calibration

## Packet shape

```json
{
  "schema_version": "oesis.flood-node.v1",
  "node_id": "flood-node-01",
  "observed_at": "2026-03-31T18:45:00Z",
  "firmware_version": "0.1.0",
  "location_mode": "outdoor",
  "install_role": "runoff_low_point",
  "sensors": {
    "mb7389": {
      "present": true,
      "analog_raw": 1876,
      "sensor_voltage_v": 1.51,
      "distance_cm": 63.2
    }
  },
  "derived": {
    "dry_reference_distance_cm": 70.0,
    "water_depth_cm": 6.8,
    "rise_rate_cm_per_hr": 1.4,
    "calibration_state": "provisional"
  },
  "health": {
    "uptime_s": 420,
    "wifi_connected": false,
    "free_heap_bytes": 214520,
    "read_failures_total": 0,
    "last_error": null
  }
}
```

## Field guidance

- `schema_version`: keep fixed at `oesis.flood-node.v1` for the first build
- `node_id`: stable per device, such as `flood-node-01`
- `observed_at`: use real UTC time when Wi-Fi/NTP is configured; otherwise the firmware may emit a placeholder timestamp
- `location_mode`: use `outdoor`
- `install_role`: use `runoff_low_point`

## Sensor fields

- `analog_raw`: raw ESP32 ADC count
- `sensor_voltage_v`: inferred voltage at the sensor output side of the divider
- `distance_cm`: provisional inferred sensor-to-surface distance

## Derived fields

- `dry_reference_distance_cm`: the dry installed distance used as the baseline
- `water_depth_cm`: `max(0, dry_reference_distance_cm - distance_cm)`
- `rise_rate_cm_per_hr`: change in derived depth over time
- `calibration_state`: `provisional` until field validation is complete

## What this contract can and cannot mean

This contract is intentionally low-point scoped.

It can support:

- depth at one documented low point
- rise / recession behavior over time
- parcel-specific runoff interpretation when installation geometry is trusted

It should **not** be treated as:

- parcel-wide flood truth from one sensor alone
- route or community flooding evidence by itself
- pump-state or action-verification evidence by itself

Later bridge and route/community stages may combine this packet family with
equipment-state, action logs, and route surfaces, but that is outside the first
contract boundary.

## Health fields

- `uptime_s`: seconds since boot
- `wifi_connected`: whether optional Wi-Fi is active
- `free_heap_bytes`: memory headroom
- `read_failures_total`: count of failed or invalid reads
- `last_error`: `null` when healthy, otherwise a short string such as `adc_read_invalid`

## Implementation status

This hardware contract is **architecturally defined**. The corresponding software
observation family (`flood.low_point.snapshot`) is implemented in the ingest path
as of the current runtime. Field teams can build flood-nodes that emit valid JSON
and the system can normalize and ingest it. See the observation family map in
`https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/integrated-parcel-system-spec.md` for overall status.

## Serial behavior

- baud rate: `115200`
- one packet per line
- optional boot comments may begin with `#`

## Example line

```json
{"schema_version":"oesis.flood-node.v1","node_id":"flood-node-01","observed_at":"2026-03-31T18:45:00Z","firmware_version":"0.1.0","location_mode":"outdoor","install_role":"runoff_low_point","sensors":{"mb7389":{"present":true,"analog_raw":1876,"sensor_voltage_v":1.51,"distance_cm":63.2}},"derived":{"dry_reference_distance_cm":70.0,"water_depth_cm":6.8,"rise_rate_cm_per_hr":1.4,"calibration_state":"provisional"},"health":{"uptime_s":420,"wifi_connected":false,"free_heap_bytes":214520,"read_failures_total":0,"last_error":null}}
```
