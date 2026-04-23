# Firmware Notes

## Responsibilities

The first thermal-pod software should:
- read a full MLX90640 frame
- compute derived metrics without persisting the frame
- emit one JSON object per line
- report basic health and timing status
- make privacy posture explicit in the packet

It should remain explicitly derived-only. It is not a shortcut to occupant
tracking, raw-frame storage, or default parcel-kit functionality.

## Sampling cadence

- start with a slow readable cadence such as every `10` seconds
- do not chase high frame rates in the first build
- prioritize stable derived metrics and thermal repeatability

## Packet shape

Use the `oesis.thermal-pod.v1` contract in `serial-json-contract.md`.

Key rules:
- no raw frame arrays in the normal serial payload
- include min, mean, max, spread, and hot fraction
- include a privacy mode or frame-retention flag
- include install role and location mode

## Error handling

- emit clear errors when the thermal frame cannot be read
- report degraded health instead of silently stalling
- make sensor-init failures obvious on startup
- keep the first-build path simple and observable

## Health telemetry

Minimum health fields:
- `uptime_s`
- `sample_interval_s`
- `read_failures_total`
- `last_error`
- optional CPU temperature or process memory if useful on Pi

## Future enhancements

- scene masking for fixed hot objects
- controlled local smoothing over multiple frames
- richer derived zones if the privacy model stays strong
- optional secured upload path from Pi to ingest
