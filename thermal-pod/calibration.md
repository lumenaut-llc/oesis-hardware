# Calibration

## Goals

The first calibration goal is a repeatable fixed scene and stable derived metrics, not laboratory-grade absolute thermography.

## Reference checks

- confirm the scene is fixed and documented
- note major surfaces in view
- record ambient conditions during early tests
- check that enclosure heat does not dominate the reported scene

## Bench calibration

- compare scene min and max against a few known warm and cool reference objects
- verify hot-fraction behavior with a deliberately introduced warm target
- check repeatability across several consecutive runs

## Outdoor validation

- validate the hood against direct sun and reflected heat
- compare derived metrics across similar times of day
- document any consistent scene bias from the install geometry
- revisit thresholds after the first real field logs exist

## Ongoing QA/QC

- recheck the scene after any mount movement
- inspect for hood contamination or enclosure heat soak
- verify no raw frames are being retained in the normal path
- compare thermal trends with other local context, not in isolation

## Open questions

- what hot-fraction threshold is actually useful
- whether scene masking belongs in config or code
- how much absolute calibration effort is justified for this derived-only node
