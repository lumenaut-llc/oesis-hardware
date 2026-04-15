# Operator Runbook

## Purpose

Provide the shortest repeatable path from first weather-pm-mast wiring to a locally validated PM-first normalized observation.

## What this runbook proves

This runbook proves the richer second-wave outdoor lane:

- the mast can emit a weather + PM packet family
- the local workflow can validate that richer packet shape
- the parcel can gain more detailed outdoor smoke / weather mechanics when the
  simpler `mast-lite` lane is no longer enough

It should not be read as the default first-kit path. This is the upgrade lane
after the simpler sheltered-outdoor node is already stable.

## Stage 1: environmental bench scan

1. Flash `firmware/weather_pm_mast_i2c_scanner/weather_pm_mast_i2c_scanner.ino`
2. Confirm `0x44` first, then `0x76` or `0x77` after the BME680 is added

## Stage 2: PM-first packet check

1. Flash `firmware/weather_pm_mast_serial_json/weather_pm_mast_serial_json.ino`
2. Confirm one JSON packet line every 5 seconds
3. Copy one packet line into `packet.json`

## Stage 3: local ingest validation

From `oesis-runtime` repo root:

```bash
python3 -m oesis.ingest.ingest_packet packet.json
```

If you captured a full serial log:

```bash
python3 -m oesis.ingest.extract_latest_packet serial.log --output packet.json
python3 -m oesis.ingest.ingest_packet packet.json
```

## Stage 4: mast move

1. Mount the environmental and PM hardware in the first outdoor mast position
2. Rerun the same packet checks after mounting
3. Watch for contamination, splash, and heat issues during a 30 to 60 minute run
