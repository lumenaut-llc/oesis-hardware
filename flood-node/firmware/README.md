# Flood Node Firmware

## Environments

- `flood_node_adc_smoke_test`
- `flood_node_serial_json`

## First bring-up order

1. build and flash `flood_node_adc_smoke_test`
2. verify the analog signal responds to target movement
3. build and flash `flood_node_serial_json`
4. capture a real packet and validate it with the ingest scripts

## Staging role

This firmware is for a geography-gated low-point hazard module. It should only
be used where parcel runoff context justifies it and should not be treated as
part of the default `v0.1` or `v0.2` kit path.

## Optional Wi-Fi time sync

Copy `flood_node_serial_json/secrets.example.h` to `flood_node_serial_json/secrets.h` and fill in credentials if you want real UTC timestamps via NTP.

If no Wi-Fi credentials are provided, the sketch still runs and uses a placeholder `observed_at`.

## Build commands

```bash
$HOME/Library/Python/3.11/bin/pio run -e flood_node_adc_smoke_test
$HOME/Library/Python/3.11/bin/pio run -e flood_node_serial_json
```
