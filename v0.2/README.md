# Hardware v0.2

`v0.2` is a promotion lane marker for the first widened parcel kit.

## Sign-off sentence

**v0.2** means: bench-air + mast-lite bound to one parcel, both streams
normalized and combined — without claiming field-hardened outdoor deployment.

## Hardware scope

| Surface | Status | Notes |
|---------|--------|-------|
| bench-air-node (indoor) | Implemented (v0.1) | Carries forward unchanged |
| mast-lite (sheltered outdoor) | Partial | Shared packet lineage; field-hardening bundle not yet closed |
| flood-node | Deferred to v0.3 | |
| weather-pm-mast | Deferred | Second-wave hardware |
| circuit-monitor | Deferred to v1.5 | |

## Blockers (from gap register)

| ID | Gap | Status |
|----|-----|--------|
| V02-G5 | Mast-lite bench bring-up: repeatable build independently reproduced | Partial |
| V02-G6 | Mast-lite field-hardening bundle | Defer — bench-only acceptable for v0.2 |
| V02-G7 | Named BOM vendors for mast-lite | Defer — not required for bench testing |

## Key acceptance criteria

- **AC-5**: Mast-lite independent bring-up — a second operator follows the build
  guide and produces a functioning node that emits valid serial JSON packets
- **AC-1**: Mast-lite packet normalization — outdoor metadata preserved through ingest

## Non-goals

- Field-hardened outdoor deployment (bench-only mast-lite is acceptable)
- Any observation family beyond air quality (no flood, thermal, equipment-state)

## How to use this lane

- Inherit baseline hardware docs from `../v0.1/`.
- Add files here only if a `v0.2`-specific hardware delta is explicitly accepted.
- Until such a delta exists, this directory is intentionally lightweight.

## Related

- `../v0.1/README.md`
- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/release/v0.2/README.md`
- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/release/v0.2/v0.2-scope-matrix.md`
- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/release/v0.2/v0.2-gap-register.md`
- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/current/pre-1.0-version-progression.md`
- `../mast-lite/README.md`
