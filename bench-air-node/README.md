# Bench Air Node

## What it is

The smallest practical indoor air-sensing node for validating the Open
Environmental Sensing and Inference System stack end to end. It is a
USB-powered ESP32-S3 build with temperature, humidity, pressure, and VOC trend
sensing intended for desk, garage, or sheltered porch use.

## Why it matters

This node is the fastest way to turn the program from concept into repeatable evidence. It lets the project validate:
- sensor bring-up
- packet timing and schema
- local health telemetry
- ingest-service expectations
- parcel operator-readable parcel evidence inputs

## Standalone value

Even without neighborhood sharing, the node gives a single parcel owner useful local conditions:
- indoor or sheltered microclimate trend
- sudden air-quality change detection
- device uptime and sensor-health monitoring
- a repeatable reference platform for later outdoor nodes

## How it connects to the larger system

The bench air node is an evidence producer, not a parcel-state engine. Its responsibility is to publish trustworthy raw and lightly processed readings with provenance so downstream services can combine them with other evidence layers. In the MVP, this node primarily informs smoke and heat-related parcel reasoning while also exercising the ingest path used by later weather, mast, and flood hardware.

Staging reference: this family is **current-truth** hardware for **program-phase `v0.1`**; see `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/node-taxonomy.md` and `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/version-and-promotion-matrix.md`.

## Scope for current version

- ESP32-S3 development board
- SHT45 breakout
- BME680 breakout
- USB power
- indoor or sheltered testing
- shared I2C bus
- JSON packet output over serial and/or Wi-Fi

## Current maturity

Default posture: `deployment maturity v0.1`

This node is the default bench and first-bring-up lane.
It becomes a `deployment maturity v1.0` candidate only when the repo documents the shared field-hardening bundle for the specific install, including a fixed harness, stable mount or enclosure posture, physical identity label, and a clear local logging or buffering posture.

## Inputs

- ambient air near the install location
- local time or monotonic uptime from firmware
- device identity and firmware version
- optional Wi-Fi credentials if packets are forwarded upstream

## Outputs

- temperature
- humidity
- pressure
- VOC/gas trend
- basic health telemetry
- packet freshness timestamp
- device and sensor status flags
- optional derived comfort or anomaly hints for UI display

## Risks and constraints

- The BME680 gas resistance value is useful for trend detection, not as a direct pollutant concentration.
- Indoor placement can be biased by HVAC vents, kitchens, bathrooms, or direct sun through windows.
- USB power keeps the build simple but does not represent the power constraints of outdoor nodes.
- I2C wiring is fine for a bench prototype but should stay short and tidy to avoid intermittent read errors.
- This node should not overclaim parcel hazards; it provides evidence with uncertainty, not final status decisions.

## Dependencies

- shared glossary
- procurement and BOM
- documentation templates
- basic ingest path on the software side
- serial JSON contract for first-build bring-up

## Required now

- short stable wiring harness
- known-good USB power and data path
- documented `node_id`
- documented install bias notes

## Add later

- fixed carrier or more stable small-board posture
- simple vented enclosure or stand
- local buffering or logging path
- stronger provisioning and status posture

## Field-ready boundary

Do not describe this node as field-ready by default.
It crosses into a `deployment maturity v1.0` claim only when the repo documents the field-hardening bundle for the specific install.

## Serviceability notes

- keep the controller and sensors accessible for inspection and reflash
- use a physical node label once the node leaves pure bench use
- keep at least one spare controller and one spare sensing path for any active field deployment

## Next milestones

- publish versioned JSON packets to the ingest service
- add ring-buffer logging for offline debugging
- define simple enclosure and airflow guidance
- add Wi-Fi provisioning and local status page
- promote the validated sensing stack into `mast-lite`

## Open questions

See `open-questions.md` for unresolved decisions on enclosure, long-term drift handling, and whether this node should emit any local anomaly scoring.

## Key docs

- `build-guide.md`
- `wiring.md`
- `firmware-notes.md`
- `serial-json-contract.md`
- `operator-runbook.md`
- `firmware/README.md`
