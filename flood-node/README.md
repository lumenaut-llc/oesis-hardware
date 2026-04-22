# Flood Node

## What it is

A dedicated low-point parcel node that measures surface distance and inferred water depth where runoff matters operationally.

Taxonomy: **geography-gated** hazard module — attach when parcel runoff context justifies it, not a universal default. See `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/node-taxonomy.md`.

## Why it matters

This node is the flood-specific complement to the other environmental builds. It lets the project validate:
- documented low-point installation discipline
- depth-oriented evidence instead of general weather context
- calibration against a real drainage geometry
- rise-rate detection during runoff events
- the difference between point runoff evidence and parcel-wide flood claims

## Standalone value

Even without the full neighborhood platform, this subsystem gives a parcel owner useful local information:
- low-point standing-water depth trend
- runoff rise and recession timing
- a repeatable way to compare storms at the same location
- early evidence for whether a chosen low point is operationally meaningful

## Scope for current version

- ESP32-S3 development board
- MB7389 ultrasonic range sensor
- weather-resistant enclosure with sensor opening or standoff
- fixed mounting bracket at a documented runoff low point
- analog-read bring-up path with install-specific calibration constants
- JSON packet output over serial and/or Wi-Fi

## Current maturity

Default posture: experimental field prototype below a general `deployment maturity v1.0` claim.

This node can be valuable early, but it should remain parcel-specific until the repo documents rigid geometry, zero reference, and repeatable service posture.

## Software implementation status

The hardware serial-JSON contract for this node (`serial-json-contract.md`) is
architecturally defined, but the corresponding software observation family
(`flood.low_point.snapshot`) is **not yet implemented** in the ingest path. The
ingest service currently normalizes only `oesis.bench-air.v1` packets. Flood-node
packets emitted by built hardware will be valid JSON but cannot be ingested or
used for parcel-state inference until the flood observation family is implemented.

See `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/release/v0.1/implementation-status-matrix.md` for current status.

## Inputs

- measured sensor-to-surface distance at the runoff low point
- install-specific dry reference distance
- local time or monotonic uptime from firmware
- device identity and firmware version
- optional Wi-Fi credentials if packets are forwarded upstream or used for time sync

## Outputs

- raw analog signal health
- inferred surface distance
- inferred water depth
- rise rate
- node health
- packet freshness timestamp
- calibration status flags

## Risks and constraints

- Flood-node evidence is only meaningful if the install point is a real runoff low point.
- Ultrasonic distance can be biased by splashing, foam, irregular surfaces, wind, and bad mounting angles.
- First-build depth numbers should be treated as provisional until dry reference and on-site checks are complete.
- This node produces point evidence, not a parcel-wide flood truth claim by itself.
- Cable routing, condensation, and enclosure splash management matter more here than in the simpler air nodes.

## Dependencies

- shared glossary
- procurement and BOM
- documentation templates
- basic ingest path on the software side
- parcel context documenting the runoff low point
- serial JSON contract for first-build bring-up

## Required now

- rigid mount that preserves geometry
- documented dry reference
- field marker or staff gauge posture
- enclosure posture that stays above likely splash or standing water
- local buffering or logging posture for event review

## Add later

- tamper or tilt detection
- stronger surge and power protection
- more formal calibration and replacement workflow

## Field-ready boundary

Do not describe the flood node as stronger parcel evidence unless mount angle, zero reference, field marker posture, and service geometry are all documented for the parcel.

## Serviceability notes

- inspect for debris, splash residue, and mount drift after storms
- keep geometry notes with the parcel record, not just in informal field memory
- maintain a spare controller and a spare ranging path for active flood parcels

## Next milestones

- prove a stable dry-reference installation
- validate depth deltas with manual ruler checks
- observe at least one real runoff event
- connect the low-point evidence to parcel flood inference conservatively

## Open questions

See `open-questions.md` for unresolved decisions on sensor interface choice, mount geometry, and how much of the depth calibration should live in firmware versus parcel context.

## Parcel kit integration

- BOM and purchase posture: `../parcel-kit/integrated-parcel-kit-bom.md` (Optional hazard module: Flood) and `../parcel-kit/parcel-kit-procurement-checklist.md` (optional node families + conditional add-ons).
- Default Tier 1–2 kit does **not** require this node; add only when the parcel has a meaningful runoff low point.

## Key docs

- `build-guide.md`
- `wiring.md`
- `firmware-notes.md`
- `serial-json-contract.md`
- `operator-runbook.md`
- `firmware/README.md`

## Platform standards alignment

flood-node's design and deployment posture are governed by platform-level standards in `oesis-program-specs`:

- **Deployment-class defaults** (outdoor class → battery+solar with 72 h runtime floor, or hardened mains / IP65 / serial → LoRa permitted) from [`deployment-maturity-ladder.md`](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/deployment-maturity-ladder.md) "Deployment-class standards".
- **Calibration posture** — characterized staff-gauge reference per [`calibration-program.md`](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/calibration-program.md) §A; no burn-in required (ultrasonic sensors do not need conditioning).
- **Protective fixtures required** per calibration-program §C item 7: **rigid mount at a documented low-point** and **zero-reference tied to a physical staff gauge**. Without documented geometry, depth numbers are not interpretable and readings are inadmissible.
- **§F metadata block** lives in [`oesis-builds/specs/flood-node/v0-1.md`](https://github.com/lumenaut-llc/oesis-builds/blob/main/specs/flood-node/v0-1.md) front-matter (skeleton pending BOM + procedures).
- **Architecture aggregator**: [`parts/flood-node.md`](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/parts/flood-node.md).
- **OSI transport figure**: Figure 8 (LoRa) in [`v0.1-osi-diagrams.md`](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/release/v.0.1/v0.1-osi-diagrams.md).
