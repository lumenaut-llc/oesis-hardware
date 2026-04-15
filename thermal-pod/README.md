# 2D Thermal Pod

## What it is

A scene-level sensing pod that uses a thermal array and publishes derived thermal metrics instead of raw frames.

Taxonomy: **research- or privacy-gated** node — outside default pilot until contract and retention posture are stronger. See `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/node-taxonomy.md`.

## Why it matters

This node is the project’s first step into area-based sensing rather than single-point measurements. It lets the project validate:
- privacy-safe derived thermal telemetry
- hooded outdoor or semi-outdoor thermal siting
- a Raspberry Pi based node path
- scene-level trend detection without storing identifiable imagery
- how much thermal context is actually useful before adding more complex sensing

## Standalone value

Even without the full neighborhood platform, this subsystem can provide:
- scene temperature spread and trend context
- hot-fraction tracking for sun load or abnormal heat sources
- early-warning thermal drift around a fixed view
- a privacy-conscious way to test scene sensing on a parcel

## Scope for current version

- Raspberry Pi 5
- MLX90640 thermal array
- hooded enclosure
- fixed field of view
- derived metrics only, no raw thermal frame persistence
- JSON line output over stdout or local log

## Current maturity

Default posture: separate R&D lane below a general `deployment maturity v1.0` claim.

This node should not inherit the deployability label of the rest of the parcel kit just because it can emit derived metrics.

## Software implementation status

The hardware serial-JSON contract for this node (`serial-json-contract.md`) is
architecturally defined, but the corresponding software observation family
(`thermal.scene.snapshot`) is **not yet implemented** in the ingest path. The
ingest service currently normalizes only `oesis.bench-air.v1` packets.
Thermal-pod packets emitted by built hardware will be valid JSON but cannot be
ingested or used for parcel-state inference until the thermal observation family
is implemented. Additionally, privacy and retention posture must be resolved
before this observation family enters the default pipeline.

See `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/release/v0.1/implementation-status-matrix.md` for current status.

## Inputs

- low-resolution thermal frame from the MLX90640
- fixed install geometry and field of view
- local system time on the Raspberry Pi
- device identity and firmware version
- optional local config for thresholds and masking

## Intended outputs

- scene max temperature
- scene mean temperature
- scene min temperature
- hot fraction
- scene spread
- thermal trend context
- node health

## Risks and constraints

- This node must stay privacy-safe by default and avoid storing raw frames in the normal first-build path.
- Thermal readings are highly sensitive to sun angle, enclosure self-heating, and background composition.
- A scene-level metric can be useful without proving parcel-wide hazard truth.
- Mounting drift or an overly broad field of view can make trends harder to interpret than point sensors.

## Dependencies

- shared glossary
- procurement and BOM
- documentation templates
- basic ingest path on the software side
- privacy-governance alignment for derived-only telemetry
- a fixed-view installation plan

## Required now

- stable Pi 5 power posture
- high-endurance storage posture
- fixed-view geometry documented in the install notes
- thermal hood or aperture posture that is repeatable
- clear privacy-safe derived-only operating mode

## Add later

- clean shutdown or backup power posture
- RTC or stronger timing posture if replay quality becomes important
- richer masking and scene-stability tooling

## Field-ready boundary

Do not describe the thermal pod as field-ready until the repo documents stable power, durable storage, clean shutdown posture, thermal isolation, and repeatable field-of-view geometry as standard requirements.

## Serviceability notes

- treat mount drift as a service event, not a minor nuisance
- keep the hood, aperture, and storage posture inspectable
- keep spare storage and power parts for any active pilot pod

## Next milestones

- prove the derived-metrics-only first build
- validate the hooded enclosure against self-heating and splash
- define scene masking and hot-fraction thresholds
- decide whether any future frame snapshots are ever justified

## Open questions

See `open-questions.md` for unresolved decisions on masking, retention, and what derived thermal fields are worth keeping.

## Parcel kit integration

- BOM and posture: `../parcel-kit/integrated-parcel-kit-bom.md` (Separate R&D lane: Thermal) and `../parcel-kit/parcel-kit-procurement-checklist.md` (optional node families).
- Keep outside the default integrated pilot bundle until privacy, retention, and usefulness boundaries are closed.

## Key docs

- `build-guide.md`
- `wiring.md`
- `firmware-notes.md`
- `serial-json-contract.md`
- `operator-runbook.md`
- `firmware/README.md`
