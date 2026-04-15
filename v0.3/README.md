# Hardware v0.3

`v0.3` is a promotion lane marker for the first flood-capable runtime.

## Sign-off sentence

**v0.3** means: flood-node observations normalized through canonical ingest,
flood conditions reflected in parcel-state — without claiming response logic,
sump monitoring, or intervention surfaces.

## Hardware scope

| Surface | Status | Notes |
|---------|--------|-------|
| bench-air-node | Implemented (v0.1) | Carries forward |
| mast-lite | Partial (v0.2 target) | Carries forward |
| flood-node (low-point sensing) | Partial | Hardware contract exists; build guide exists; independent reproduction not confirmed |
| weather-pm-mast | Deferred | Second-wave hardware |
| circuit-monitor | Deferred to v1.5 | |

## Blockers (from gap register)

| ID | Gap | Status |
|----|-----|--------|
| V03-G5 | Flood-node independent build reproduction | Partial — build guide exists; not independently confirmed |
| V03-G6 | Flood-node calibration: dry reference and depth derivation | Provisional — calibration documented but not field-validated |
| V03-G7 | Flood-node field deployment context | Defer — not required for bench testing |

## Key acceptance criteria

- **AC-5**: Flood-node independent bring-up — second operator produces functioning
  node emitting valid serial JSON with distance sensor readings and derived water depth
- **AC-1**: Flood packet normalization — flood-specific fields preserved through ingest

## Non-goals

- Sump pump monitoring or equipment-state observation
- Flood intervention logic or response verification
- Weather-PM or thermal observation families

## How to use this lane

- Inherit baseline hardware docs from `../v0.1/`.
- Add files here only if a `v0.3`-specific hardware delta is explicitly accepted.

## Related

- `../v0.2/README.md`
- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/release/v0.3/README.md`
- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/release/v0.3/v0.3-scope-matrix.md`
- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/release/v0.3/v0.3-gap-register.md`
- `../flood-node/README.md`
