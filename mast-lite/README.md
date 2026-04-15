# Mast-Lite Outdoor Node

## What it is

A simple outdoor parcel node focused on stable environmental readings, basic weatherproofing, and first sheltered outdoor deployment.

## Why it matters

This node is the natural outdoor follow-on to the bench air node. It lets the project validate:
- outdoor sensor siting
- radiation shielding and enclosure bias
- versioned packet continuity across indoor and outdoor nodes
- longer cable and mounting realities
- first parcel-edge evidence from sheltered outdoor conditions

## Standalone value

Even without neighborhood sharing, this node gives a single parcel owner useful local conditions:
- sheltered outdoor microclimate trend
- pressure and humidity trend context for smoke, heat, and storm interpretation
- a repeatable outdoor reference platform before the full weather mast is built
- early enclosure and maintenance lessons for later nodes

## Scope for current version

- ESP32-S3 development board
- SHT45 in radiation shield
- BME680
- optional UV
- vented outdoor enclosure
- basic mast or eave mounting
- shared I2C bus
- JSON packet output over serial and/or Wi-Fi

## Current maturity

Default posture: early `deployment maturity v1.0` target, but not field-ready by default.

This node is the first outdoor critical-path lane for the parcel kit, which means enclosure, mounting, buffering, and serviceability are part of the node definition rather than optional extras.

**Program posture:** `mast-lite` is part of the **next promotion** integrated kit (**program-phase `v0.2`**) together with `bench-air-node`. It is **not** the same as **current truth** until that slice is promoted. In the reference runtime, shared-lineage normalization may remain **partial** until promotion — see `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/integrated-parcel-system-spec.md` (observation family map) and `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/version-and-promotion-matrix.md`.

## Inputs

- ambient sheltered outdoor air near the install location
- local time or monotonic uptime from firmware
- device identity and firmware version
- optional Wi-Fi credentials if packets are forwarded upstream or used for time sync

## Outputs

- temperature
- humidity
- pressure
- VOC/gas trend
- optional UV
- node health
- packet freshness timestamp
- device and sensor status flags

## Risks and constraints

- The BME680 gas resistance value is useful for trend detection, not as a direct pollutant concentration.
- Outdoor siting errors can dominate sensor quality even when wiring and firmware are correct.
- Direct sun, reflected heat, wet surfaces, and wall radiation can bias temperature badly without shielding and spacing.
- A vented enclosure improves survivability but can still trap heat or moisture if airflow is poor.
- This node should publish evidence with uncertainty, not parcel hazard claims.

## Dependencies

- shared glossary
- procurement and BOM
- documentation templates
- basic ingest path on the software side
- sheltered outdoor mounting assumptions
- serial JSON contract for first-build bring-up

## Required now

- protected outdoor or semi-outdoor power posture
- vented enclosure with cable-gland and moisture posture
- connectorized or otherwise stable sensor leads
- local buffering or durable storage posture
- sheltered siting discipline documented in install notes

## Add later

- stronger watchdog and device-health posture
- external antenna options where enclosure placement hurts radio quality
- richer calibration and maintenance logging

## Field-ready boundary

Do not call `mast-lite` field-ready unless the repo documents protected power, cable glands, venting, connectorized wiring, local buffering, physical label, and sheltered install discipline for the active parcel.

## Serviceability notes

- keep enclosure access practical after mounting
- document vent, gland, and cable-routing choices in the install record
- keep one spare controller and one spare environmental sensing path for active parcels

## Next milestones

- validate the sheltered outdoor build against the bench air node packet contract
- add UV only after the base environmental stack is stable
- improve enclosure and cable routing
- promote the validated outdoor sensing stack into `weather-pm-mast`

## Open questions

See `open-questions.md` for unresolved decisions on enclosure venting, UV inclusion, and how aggressively outdoor filtering should happen on-node.

## Key docs

- `build-guide.md`
- `wiring.md`
- `firmware-notes.md`
- `serial-json-contract.md`
- `operator-runbook.md`
- `firmware/README.md`
