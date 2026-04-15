# Hardware

Hardware subsystems are designed as standalone builds that also become inputs to the broader parcel platform.

Read `NOTICE.md` before treating this subtree as a complete released hardware design package.

## Lane contract

- **Baseline lane**: root subsystem docs in `../` (`bench-air-node/`,
  `mast-lite/`, `flood-node/`, `weather-pm-mast/`, `thermal-pod/`,
  `parcel-kit/`) represent the accepted baseline hardware posture.
- **Lane index role**: this `v0.1/` directory provides baseline entrypoints and
  compatibility mapping to those canonical baseline docs.
- **Additive lanes**: `../v1.0/` and `../v1.5/` capture forward-lane hardware deltas
  and bridge-stage families.
- **Compatibility policy**: keep redirect pointers when hardware docs move across
  lanes.
- **Canonical scope mapping**: lane claims must align with
  `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/version-and-promotion-matrix.md` and deployment
  maturity rules.

## Design rules

- keep nodes modular
- separate sensors when placement requirements differ
- prefer stable, documented modules over custom boards in v0.x
- keep each node useful on its own
- publish calibration and maintenance notes alongside build steps
- distinguish bench prototypes from field-ready nodes with an explicit deployment maturity label
- treat field-hardening parts as part of the node, not as optional support gear once a node is described as deployed

## Cross-subsystem reference

Use `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/integrated-parcel-system-spec.md` as the canonical document for how all current hardware families attach to one parcel, one software stack, and one governance surface.
Use `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/node-taxonomy.md` for **planned and geography-gated** node families, **v1.5 bridge** hardware names, and **non-node** evidence surfaces — with explicit staging so taxonomy is not mistaken for shipped kits.
Use `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/version-and-promotion-matrix.md` for how **program-phase** slices (`v0.1`, `v0.2`, …), **capability stages**, and **deployment maturity** relate.
Use `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/current/README.md` as the current versioned
reference-architecture entrypoint for the broader system.
Use `../v1.0/` and `../v1.5/` in this subtree when hardware docs need explicit
version-lane placement beyond the frozen `v0.1` baseline.
Use `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/deployment-maturity-ladder.md` and `../parcel-kit/field-hardening-checklist.md` as the canonical references for when a node family is still `deployment maturity v0.1` versus ready to target `deployment maturity v1.0` or above.

## Node families

| Directory | Role |
| --- | --- |
| `bench-air-node/` | Indoor or sheltered bench reference node (`oesis.bench-air.v1` lineage) |
| `mast-lite/` | Sheltered outdoor air node, same packet lineage as bench-air |
| `flood-node/` | Optional low-point runoff depth evidence |
| `weather-pm-mast/` | Optional richer outdoor PM and weather mast (second-wave) |
| `thermal-pod/` | Optional scene-level thermal R&D (Pi + MLX90640, derived-only posture) |
| `parcel-kit/` | Integrated BOM, procurement, installation, and field-hardening checklists |

Planned **v1.5 bridge** hardware families (`indoor-response-node`, `power-outage-node`, `freeze-node`) are defined in `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/node-taxonomy.md`; create matching subtrees when there is a build guide and contract, not before.

Start kit planning in `../parcel-kit/integrated-parcel-kit-bom.md` and `../parcel-kit/parcel-kit-procurement-checklist.md`.

## Baseline family entrypoints

- `../bench-air-node/README.md`
- `../mast-lite/README.md`
- `../flood-node/README.md`
- `../weather-pm-mast/README.md`
- `../thermal-pod/README.md`
- `../parcel-kit/README.md`
