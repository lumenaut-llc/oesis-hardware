# Thermal Pod Firmware

## Runtime model

This node uses a Raspberry Pi and Python rather than an ESP32 firmware upload flow.

## Staging role

This is a research- and privacy-gated software path. It is useful for derived
scene-level thermal experimentation, but it should stay outside the default
parcel-kit baseline and outside the first `v0.2` promotion target.

## Files

- `thermal_pod_frame_probe.py`
- `thermal_pod_serial_json.py`
- `requirements.txt`
- `config.example.json`

## First bring-up order

1. install dependencies
2. run `thermal_pod_frame_probe.py --once`
3. run `thermal_pod_serial_json.py --once`
4. save a sample packet and validate it locally

## Privacy posture

- normal output is derived metrics only
- no raw thermal frame should be written to disk in the standard path
- any future change to that rule should be treated as a separate design decision
