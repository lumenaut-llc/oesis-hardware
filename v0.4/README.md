# Hardware v0.4

`v0.4` is a promotion lane marker for multi-node registry and evidence composition.

## Sign-off sentence

**v0.4** means: mature node lifecycle, installation metadata, and
deployment-quality flags — without claiming trust scoring as a product surface
or governance enforcement.

## Hardware scope

| Surface | Status | Notes |
|---------|--------|-------|
| bench-air-node | Implemented | Carries forward |
| mast-lite | v0.2 target | Carries forward |
| flood-node (where installed) | v0.3 target | Carries forward |
| weather-pm-mast | Deferred | Second-wave; may contribute to later pre-1.0 or v1.0 |
| circuit-monitor | Deferred to v1.5 | |

No new hardware families at this slice. v0.4 is primarily a software and
registry lifecycle slice; hardware impact is through metadata and deployment
maturity labeling.

## Key hardware-relevant items

| ID | Item | Status |
|----|------|--------|
| V04-G2 | Installation metadata capture (mount type, height, orientation, exposure, power source) | Docs-only — contract formalized; no capture surface |
| V04-G4 | Deployment-quality flags (bench-grade vs field-ready) visible per node | Planned — maturity ladder documented; not surfaced in runtime |

## Key acceptance criteria

- **AC-2**: Installation metadata capture — operator submits deployment metadata;
  stored as valid `deployment-metadata` record linked to node's `install_record_id`
- **AC-4**: Deployment-quality flags visible — parcel view surfaces bench-grade vs
  field-validated distinction for each contributing node

## Carry-forward gaps

- V04-G7: Mast-lite field-hardening (deferred from v0.2, track for v1.0)
- V04-G10: Named BOM vendors (carry-forward, track for v1.0)

## How to use this lane

- Inherit baseline hardware docs from `../v0.1/`.
- Add files here only if a `v0.4`-specific hardware delta is explicitly accepted.

## Related

- `../v0.3/README.md`
- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/release/v0.4/README.md`
- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/release/v0.4/v0.4-scope-matrix.md`
- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/release/v0.4/v0.4-gap-register.md`
- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/deployment-maturity-ladder.md`
