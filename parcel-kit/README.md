# Build Guides

## Purpose

Cross-link index for all hardware build guides and installation notes.

## Read with the promotion matrix

Kit tiers here are **build tiers**, not automatic proof of an accepted program-phase slice. **Tier 1** aligns with **`v0.1`**; **Tier 2** (bench-air + `mast-lite`) is the **`v0.2` next-promotion** target until the promotion bar is met. See `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/version-and-promotion-matrix.md` and `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/node-taxonomy.md`.

## Minimum contents

- subsystem build guides
- cross-subsystem kit BOMs
- install and mounting references

## Current status

- `integrated-parcel-kit-bom.md` defines the first parcel-kit BOM by deployment tier.
- `procurement-and-bom.csv` preserves the starter-package quick-buy CSV alongside the other parcel-kit docs in this directory.
- `parcel-kit-procurement-checklist.md` defines what to buy now for Tier 1 and Tier 2 parcel kits.
- `parcel-installation-checklist.md` defines where and how to place the first parcel-kit nodes.
- `integrated-parcel-builder-checklist.md` defines the cross-layer build checklist for hardware, firmware, ingest, inference, parcel UX, and governance.
- `field-hardening-checklist.md` defines the shared support bundle required before a node is described as deployed or field-ready.
- `pilot-field-kit.md` defines the supporting parts and spares needed around the active parcel nodes.
- Optional node families (`../flood-node/`, `../weather-pm-mast/`, `../thermal-pod/`) are documented in `integrated-parcel-kit-bom.md` and in `parcel-kit-procurement-checklist.md` under **Optional node families** when you build beyond Tier 1–2.

## Related workstreams

- hardware node families
- system overview
- data model

## Next docs to add

- node retirement and replacement checklist
