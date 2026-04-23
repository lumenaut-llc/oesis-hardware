# Build Guide

## Summary

Build a PM-first outdoor mast node around an ESP32-S3 DevKitC-1, one SPS30, one SHT45, and one BME680. The first-build goal is a stable particulate and environmental mast prototype before wind and rain mechanics are added.

## Current maturity

Default posture: second-wave node with a `deployment maturity v1.5` target rather than a default `deployment maturity v1.0` requirement.

**Program posture:** this is a second-wave outdoor lane after the simpler
`mast-lite` path is stable. It should be treated as an upgrade beyond the
`v0.2` two-node parcel kit, not as part of the current-truth baseline or a
requirement for the first integrated promotion. See
`https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/node-taxonomy.md`.

## What this build actually adds

Relative to `mast-lite`, this build is for richer outdoor mechanics where the
parcel really benefits from them:

- PM-based outdoor smoke evidence
- more detailed exposed outdoor weather behavior
- a better platform for wind / rain expansion later

It is appropriate when the simpler outdoor lane is already stable and the next
question is not merely “what is the sheltered parcel edge like?” but “do we
need richer outdoor PM and weather mechanics at this parcel or geography?”

It should not be used to skip the simpler two-node parcel kit when that kit has
not yet been proven.

## Required now

- stable 5V PM power posture
- deliberate airflow and splash posture
- mast-mount and enclosure service posture
- local buffering or durable storage if the node will be described as deployed
- maintenance and cleaning posture for the PM path

## Add later

- wind and rain interface posture
- removable service module for the PM lane
- stronger maintenance counters and device-health posture

## Field-ready boundary

Do not describe this node as field-ready unless the repo documents the PM power path, airflow path, interface posture, buffering, and service-module posture.

## Serviceability notes

- particulate maintenance is part of the node definition
- keep airflow and splash decisions documented alongside the install record

## Required tools

- soldering iron with fine tip
- solder
- flush cutters and small pliers
- multimeter
- USB-C cable for programming and power
- enclosure and mast drilling tools as needed
- temporary bench wiring tools before final mast assembly

## Required materials

- 1 ESP32-S3 DevKitC-1 with 3.3 V logic
- 1 SPS30 particulate sensor
- 1 SHT45 breakout
- 1 BME680 breakout
- 1 radiation shield or similarly shaded vented mount for the SHT45
- 1 vented outdoor enclosure
- mast hardware and mounting brackets
- optional wind and rain sensors only after the PM-first build is stable

## Assembly steps

1. Validate the environmental stack on the bench first.
2. Add the SPS30 after the controller and I2C bus are stable.
3. Keep `GPIO8` for `SDA` and `GPIO9` for `SCL` on the environmental bus.
4. Mount the SPS30 and enclosure so airflow is not blocked and water cannot drain into the sensor path.
5. Keep the SHT45 in the cleanest shielded airflow position on the mast.
6. Move to mast mounting only after the serial packet flow is stable on the bench.

## Wiring notes

- Keep particulate airflow paths clean and free of cable obstructions.
- Keep the SHT45 away from enclosure heat and from exhaust or disturbed air around the SPS30.
- Keep the BME680 inside the environmental support role rather than the primary temperature reference.
- Add wind/rain mechanics only after the PM-first stack is stable.

## Firmware setup

1. Assign a stable `node_id` such as `weather-pm-mast-01`.
2. Start with the scanner sketch, then the serial JSON sketch.
3. Use a PM-first packet shape that includes environmental data plus `sps30`.
4. Keep wind and rain fields out of the first packet until those sensors are truly wired and calibrated.

The exact first-build serial payload target is defined in `serial-json-contract.md`.

## First test procedure

1. Confirm the environmental and PM stack is stable on the bench.
2. Confirm the serial JSON sketch prints valid packets every 5 seconds.
3. Move to the outdoor mast and rerun the same packet checks.
4. Leave the node running for 30 to 60 minutes and watch for resets, vanishing sensors, or implausible PM spikes.
5. Inspect airflow and enclosure heat behavior before treating the node as field-ready.

## Field-test notes

- Use sheltered or at least thoughtfully drained placement first.
- Keep the PM inlet away from splash zones, direct exhaust, and stagnant trapped air.
- Expect maintenance and cleaning to matter more than with the simpler nodes.

## Maintenance

- inspect particulate inlets and vents for dust loading
- inspect cable strain relief and mast fasteners after wind events
- rerun a short packet-health check after enclosure changes
- document cleaning intervals for the SPS30

## Links to related docs

- `README.md`
- `wiring.md`
- `firmware-notes.md`
- `serial-json-contract.md`
- `operator-runbook.md`
- `firmware/README.md`
- `calibration.md`
