# Hardware v0.1

## Purpose

Canonical baseline hardware lane for OESIS. Every node family, cross-cutting concern (calibration, parcel-kit), and build artifact referenced as "baseline" by later lanes (v0.2…v1.5) lives here.

Read `../NOTICE.md` before treating this subtree as a complete released hardware design package.

## Repository-layout rule

This lane holds the **canonical content**. Later version lanes (`../v0.2/` through `../v1.0/`) are lane-index manifestos that inherit this baseline; they contain per-slice sign-off sentences, acceptance criteria, and delta references — not duplicated hardware docs.

`../v1.5/` is the bridge-stage additive lane (indoor-response-node, power-outage-node, equipment-state-adapter, circuit-monitor).

## Node families

| Directory | Role |
| --- | --- |
| [`bench-air-node/`](bench-air-node/) | Indoor or sheltered bench reference node (`oesis.bench-air.v1` lineage) |
| [`mast-lite/`](mast-lite/) | Sheltered outdoor air node, same packet lineage as bench-air |
| [`flood-node/`](flood-node/) | Optional low-point runoff depth evidence |
| [`weather-pm-mast/`](weather-pm-mast/) | Optional richer outdoor PM and weather mast (second-wave) |
| [`thermal-pod/`](thermal-pod/) | Optional scene-level thermal R&D (Pi + MLX90640, derived-only posture) |

Planned **v1.5 bridge** hardware families (`indoor-response-node`, `power-outage-node`, `freeze-node`, `equipment-state-adapter`, `circuit-monitor`) live in `../v1.5/` when their build guide and contract exist, not before. See [`node-taxonomy.md`](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/node-taxonomy.md) for the full staging.

## Parcel kit

[`parcel-kit/`](parcel-kit/) contains cross-node resources:

- [`integrated-parcel-kit-bom.md`](parcel-kit/integrated-parcel-kit-bom.md)
- [`power-source-guide.md`](parcel-kit/power-source-guide.md)
- [`field-hardening-checklist.md`](parcel-kit/field-hardening-checklist.md)
- [`parcel-installation-checklist.md`](parcel-kit/parcel-installation-checklist.md)
- [`parcel-kit-procurement-checklist.md`](parcel-kit/parcel-kit-procurement-checklist.md)

## Calibration

[`calibration/`](calibration/) contains the node-family-agnostic calibration posture. Per-family calibration procedures live inside each node family's dir.

## Design rules

- keep nodes modular
- separate sensors when placement requirements differ
- prefer stable, documented modules over custom boards in v0.x
- keep each node useful on its own
- publish calibration and maintenance notes alongside build steps
- distinguish bench prototypes from field-ready nodes with an explicit deployment maturity label
- treat field-hardening parts as part of the node, not as optional support gear once a node is described as deployed

## Cross-subsystem references

- [`integrated-parcel-system-spec.md`](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/integrated-parcel-system-spec.md) — how all hardware families attach to one parcel, one software stack, one governance surface.
- [`node-taxonomy.md`](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/node-taxonomy.md) — planned and geography-gated node families; v1.5 bridge hardware names; non-node evidence surfaces.
- [`version-and-promotion-matrix.md`](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/version-and-promotion-matrix.md) — how program-phase slices (`v0.1`, `v0.2`, …), capability stages, and deployment maturity relate.
- [`architecture/current/`](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/current/README.md) — current versioned reference-architecture entrypoint.
- [`deployment-maturity-ladder.md`](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/deployment-maturity-ladder.md) and [`parcel-kit/field-hardening-checklist.md`](parcel-kit/field-hardening-checklist.md) — when a node family is still `deployment maturity v0.1` vs ready for `v1.0` or above.
