# Calibration

## Goals

The first calibration goal is not perfect absolute depth. It is a trustworthy dry reference and a stable change signal at the documented runoff low point.

## Reference checks

- confirm the sensor face and target surface geometry are stable
- record the dry surface distance with a ruler or tape
- note sensor angle, mount height, and what surface is actually being measured
- verify the analog signal changes monotonically with target distance on the bench

## Bench calibration

- start with provisional firmware constants for analog scaling
- compare reported distance against a few ruler-checked target positions
- adjust the linear conversion only enough to keep the first build plausible
- do not overfit bench numbers if the final field geometry will differ

## Outdoor validation

- capture the dry installed reference distance immediately after mounting
- compare shallow water or controlled target checks against the reported depth
- document any persistent bias caused by mount angle or surface reflectivity
- wait for at least one real runoff event before treating filtering choices as settled

## Ongoing QA/QC

- recheck dry-reference distance after any remount or enclosure change
- inspect for dirt, splash residue, or vegetation affecting the beam path
- compare flood-node trends against rainfall context and site notes, not just raw numbers
- keep field logs from early storm events for later calibration refinement

## Open questions

- how much of the calibration should live in firmware constants versus parcel context metadata
- whether the final preferred interface is analog only or another MB7389 output mode
- what level of manual verification is required before depth supports stronger inference
