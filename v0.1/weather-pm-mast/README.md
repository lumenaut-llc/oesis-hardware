# Weather + PM Mast

## What it is

A more complete parcel mast that adds particulate sensing and first local weather mechanics on top of the simpler outdoor node pattern.

Taxonomy: **second-wave / geography-justified** outdoor module after `mast-lite` is stable — see `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/node-taxonomy.md`.

## Why it matters

This node is the bridge from basic parcel-edge weather evidence into richer smoke and storm context. It lets the project validate:
- particulate sensing on the parcel
- mast wiring and enclosure discipline
- packet growth beyond the simpler environmental nodes
- maintenance expectations for exposed outdoor hardware
- first local weather-mechanics evidence before a fully mature field station

## Standalone value

Even without neighborhood sharing, this subsystem gives a parcel owner useful local conditions:
- PM1/2.5/4/10 trend visibility
- local weather context for smoke and storm interpretation
- a mast platform that can grow from a simpler PM-first build into a fuller outdoor station
- operational lessons for maintenance, cleaning, and weather exposure

## Scope for current version

- ESP32-S3 development board
- SHT45 in radiation shield
- BME680
- SPS30
- mast hardware
- vented outdoor enclosure
- optional wind speed, wind direction, and rainfall after the PM-first build is stable
- JSON packet output over serial and/or Wi-Fi

## Current maturity

Default posture: second-wave node with a `deployment maturity v1.5` target rather than a default `deployment maturity v1.0` requirement.

This lane raises the bar for power, airflow, maintenance, buffering, and serviceability relative to `mast-lite`.

## Software implementation status

The hardware serial-JSON contract for this node (`serial-json-contract.md`) is
architecturally defined, but the corresponding software observation family
(`air.pm_weather.snapshot`) is **not yet implemented** in the ingest path. The
ingest service currently normalizes only `oesis.bench-air.v1` packets.
Weather-pm-mast packets emitted by built hardware will be valid JSON but cannot
be ingested or used for parcel-state inference until the PM/weather observation
family is implemented.

See `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/release/v0.1/implementation-status-matrix.md` for current status.

## Inputs

- ambient sheltered or mast-mounted outdoor air
- particulate counts and mass trends from the SPS30
- local time or monotonic uptime from firmware
- device identity and firmware version
- optional Wi-Fi credentials if packets are forwarded upstream or used for time sync

## Intended outputs

- PM1/2.5/4/10
- temperature
- humidity
- pressure
- VOC/gas trend
- optional wind
- optional rain
- node health
- packet freshness timestamp
- environmental context for smoke and storm logic

## Risks and constraints

- PM sensors add airflow, contamination, and maintenance concerns that simpler nodes do not have.
- Direct rain, splash, and dust loading can degrade both survivability and data quality quickly.
- Wind and rain mechanics should not be treated as solved in the first PM-first build unless they are actually instrumented and calibrated.
- This node should publish evidence with uncertainty, not parcel hazard claims.

## Dependencies

- shared glossary
- procurement and BOM
- documentation templates
- basic ingest path on the software side
- mast-lite learnings for outdoor enclosure and siting
- serial JSON contract for first-build bring-up

## Required now

- stable 5V PM power posture
- deliberate airflow and splash posture for the SPS30
- mast mounting and enclosure service posture
- local buffering or durable local storage
- maintenance and cleaning posture for the particulate lane

## Add later

- wind and rain interface posture
- removable service module for the PM path
- stronger device-health and maintenance counters

## Field-ready boundary

Do not describe `weather-pm-mast` as field-ready unless the repo documents the PM power path, weather-interface posture, airflow path, local buffering, physical label, and service-module posture for the active configuration.

## Serviceability notes

- particulate maintenance is part of the node, not an optional later consideration
- keep airflow and splash decisions documented alongside the install record
- maintain spare controller, PM path, and enclosure support parts for active parcels

## Next milestones

- prove a PM-first mast build with stable packet output
- add wind and rain mechanics only after the PM/environment core is stable
- define maintenance and cleaning cadence for SPS30
- integrate the richer evidence stream into parcel inference

## Open questions

See `open-questions.md` for unresolved decisions on PM-first versus full weather-first scope, mast mechanics sequencing, and field maintenance expectations.

## Parcel kit integration

- BOM and purchase posture: `../parcel-kit/integrated-parcel-kit-bom.md` (Tier 3: Rich outdoor upgrade) and `../parcel-kit/parcel-kit-procurement-checklist.md` (optional node families + conditional add-ons).
- Treat as an upgrade after `mast-lite` is stable, not a prerequisite for the first integrated pilot.

## Key docs

- `build-guide.md`
- `wiring.md`
- `firmware-notes.md`
- `serial-json-contract.md`
- `operator-runbook.md`
- `firmware/README.md`
