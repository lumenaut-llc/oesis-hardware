# Calibration

## Goals

- verify that the sheltered outdoor node is stable enough to trust as parcel evidence input
- separate real environmental changes from siting, enclosure, or radiation artifacts
- document uncertainty instead of pretending to fully calibrate consumer sensors outdoors

## Reference checks

- compare SHT45 temperature and humidity against a known-good sheltered outdoor reference
- compare BME680 pressure against a trusted local weather reference after adjusting for station differences
- record sun, shade, wind shielding, and mounting geometry during every comparison

## Bench calibration

1. Validate the full sensor stack indoors first.
2. Record at least 15 minutes of stable readings before moving outdoors.
3. Confirm the SHT45 remains the primary temperature and humidity reference in packet outputs.
4. Treat BME680 gas resistance as a baseline trend source only.

## Outdoor validation

1. Mount the node in a sheltered outdoor location.
2. Let the node stabilize for at least 30 minutes after placement.
3. Record 30 to 60 minutes of readings at a fixed location.
4. Compare trends against a nearby trusted reference if one exists.
5. Watch for direct-sun bias, wall-radiation bias, and enclosure heat soak.

## Ongoing QA/QC

- inspect for drift after hot days, direct sun incidents, or heavy humidity exposure
- rerun a short side-by-side check after any enclosure or mounting change
- document any offsets or siting caveats in the node changelog

## Open questions

- How much outdoor filtering belongs in firmware versus downstream normalization?
- At what point does enclosure heat soak make the node unsuitable for a given location?
- When should UV be added instead of kept out of the first mast-lite build?
