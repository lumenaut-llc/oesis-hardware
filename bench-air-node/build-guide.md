# Build Guide

## Summary

Build a USB-powered indoor air node around an ESP32-S3 DevKitC-1 with one SHT45 breakout and one BME680 breakout on the same I2C bus. The first-build goal is a stable bench prototype that proves basic wiring, I2C detection, and repeatable sensor reads before any enclosure or transport complexity is added.

## Current maturity

Default posture: `deployment maturity v0.1`

Use this guide as the bench and first-bring-up path.
Do not read it as a field-ready guide unless the install also satisfies the shared field-hardening checklist.

**Program posture:** this is the smallest **current-truth** hardware build for the
parcel-sensing baseline and the indoor anchor for **program-phase `v0.1`**.
It remains the first build before the sheltered-outdoor `mast-lite` lane. The
later **`v1.5`** bridge additions named in the product taxonomy
(`indoor-response-node`, `power-outage-node`, equipment-state, action/outcome
verification) are **not** covered by this guide yet.

## What this build actually proves

This build proves the minimum indoor evidence path for the parcel-first system:

- stable sensor bring-up
- repeatable packet emission
- parcel-local indoor or sheltered evidence
- first end-to-end ingest → normalized observation → parcel-state flow

It does **not** yet prove:

- outdoor parcel-edge conditions
- direct PM-based smoke observation
- household equipment-state awareness
- intervention tracking
- outcome verification

## How this build fits the next stages

This guide is still the right first hardware step even under the newer product
direction. The later bridge toward adaptation depends on having a stable indoor
evidence lineage before adding more complex house-state surfaces.

In the newer staged product scope, the closest follow-on roles are:

- `mast-lite` for sheltered outdoor reference conditions in **`v0.2`**
- later `indoor-response-node` for indoor PM2.5 + temperature + RH in **`v1.5`**
- later equipment-state and action/outcome surfaces that explain what the house
  did while indoor conditions were changing

## Required now

- short stable wiring
- known-good USB power and data path
- documented `node_id`
- simple physical stability so the sensing stack is not flopping loose on the bench

## Add later

- fixed harness or small carrier posture
- simple enclosure or stand
- local buffering or logging posture
- physical label once the node leaves pure bench use

## Field-ready boundary

This guide alone does not make the node field-ready.
Use `../parcel-kit/field-hardening-checklist.md` before describing a bench-air install as deployed.

## Serviceability notes

- keep the controller and sensors accessible for inspection and reflash
- keep one spare controller and one spare sensing path for any active field deployment

## Required tools

- soldering iron with fine tip
- solder
- flush cutters and small pliers
- breadboard or small perfboard
- multimeter
- USB-C cable for programming and power

## Required materials

- 1 ESP32-S3 DevKitC-1 with 3.3 V logic
- 1 SHT45 breakout
- 1 BME680 breakout
- jumper wires, preferably short
- breadboard or perfboard and headers
- optional standoffs or a small vented enclosure for sheltered use

## Preflight

1. Confirm you have a known-good USB data cable, not a power-only cable.
2. Confirm the board powers on and appears in the host flashing tool before adding sensors.
3. Inspect sensor headers and solder joints before wiring; rework any dull, cracked, or misaligned joint now.
4. Plan a compact 4-row bus on the breadboard for `3V3`, `GND`, `SDA`, and `SCL`.

## Assembly steps

1. Solder headers to the sensor breakout boards using the breadboard to keep the pins straight.
2. Seat the ESP32-S3 across the breadboard center gap.
3. Create a compact 4-row shared bus for `3V3`, `GND`, `SDA`, and `SCL`.
4. Wire the ESP32-S3 to that bus with the project pin map: `3V3`, `GND`, `GPIO8` for `SDA`, and `GPIO9` for `SCL`.
5. Wire the SHT45 only.
6. Power the board over USB and run an I2C scanner.
7. Stop and fix wiring if the scanner does not find the SHT45 at `0x44`.
8. Run an SHT45-only read test and confirm stable temperature and humidity readings.
9. Power the board off.
10. Add the BME680 to the same shared bus.
11. Power back on and run the I2C scanner again.
12. Continue only if the scanner sees the SHT45 at `0x44` and the BME680 at `0x76` or `0x77`, depending on breakout configuration.
13. Run the combined read test and verify plausible values from both sensors.
14. Leave the node running for 30 to 60 minutes before treating the bench build as passed.

## Wiring notes

- Both sensors share the same I2C bus.
- Use 3.3 V power only unless a specific breakout board explicitly documents safe regulation and level shifting.
- Start with bench wiring under 20 cm total per sensor lead.
- Avoid sealing the sensors inside an airtight case; both require airflow to be useful.
- Breakout silkscreens vary. `VIN`, `Vin`, or `3V3` all mean the power input on many boards.
- Many BME680 breakout boards label I2C pins as `SDI` for data and `SCK` for clock. In this build, `SDI` goes to `SDA` and `SCK` goes to `SCL`.

## Firmware setup

1. Assign a stable `node_id` such as `bench-air-01`.
2. Start with serial logging and scanner sketches before enabling any Wi-Fi transport.
3. Use `GPIO8` for `SDA` and `GPIO9` for `SCL` in the firmware configuration and example code.
4. Set the sample cadence to 5 seconds for interactive bring-up, 15 seconds for bench validation, or 60 seconds for quiet long-run testing.
5. Treat the SHT45 as the primary temperature and humidity reference during bring-up.
6. Treat BME680 gas resistance as a trend signal, not a calibrated air-quality number.
7. Enable serial JSON output first.
8. If Wi-Fi transport is enabled, keep serial logging on until the ingest path is stable.
9. Store firmware version, board type, and sensor presence in every boot log and periodic health message.

The exact first-build serial payload target is defined in `serial-json-contract.md`.
The first firmware scaffold lives in `firmware/bench_air_node_serial_json/bench_air_node_serial_json.ino`.

## First test procedure

1. With only the SHT45 connected, boot the node and confirm the scanner detects `0x44`.
2. Run the SHT45-only test and confirm stable temperature and humidity values for at least several read cycles.
3. Power off and add the BME680 to the same bus.
4. Boot again and confirm the scanner now detects `0x44` and `0x76` or `0x77`.
5. Run the combined test and verify temperature, humidity, and pressure values are plausible for the room.
6. Confirm BME680 gas resistance is nonzero and changing gradually rather than remaining fixed.
7. Leave the node stationary for 30 to 60 minutes and inspect drift, packet continuity, and any missing-device events.
8. Trigger a gentle step change by breathing near the sensors briefly or moving the node to another room.
9. Confirm the packets show a clear trend without sensor lockups or resets.

## Acceptance criteria

- SHT45 is detected by itself at `0x44`.
- BME680 is detected on the shared bus at `0x76` or `0x77`.
- SHT45-only reads are stable for multiple cycles.
- Combined reads remain plausible for 30 to 60 minutes with no freezes, resets, or disappearing devices.
- Sensor placement keeps both breakouts slightly away from the ESP32 regulator and USB connector heat.

## Field-test notes

This version is for indoor or sheltered testing only. If it is used on a porch or in a garage, avoid direct rain, direct sun on the enclosure, and positions immediately adjacent to HVAC exhaust or combustion sources unless that bias is intentional for a test.

## Maintenance

- inspect wiring whenever packet dropouts or missing sensor reads appear
- recheck sensor detection after firmware changes
- keep vents and sensor faces free of dust
- repeat a short reference check after any major relocation

## Safety notes

- Power only from known-good USB sources.
- Do not place the node where condensation, splashes, or conductive debris can reach exposed wiring.
- Do not use this prototype as a life-safety detector.

## Links to related docs

- `README.md`
- `wiring.md`
- `firmware-notes.md`
- `serial-json-contract.md`
- `operator-runbook.md`
- `firmware/README.md`
- `calibration.md`
