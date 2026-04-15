# Calibration

## Goals

- verify that the node is stable enough to trust as parcel evidence input
- separate real environmental changes from assembly or placement artifacts
- document uncertainty rather than pretending to fully calibrate consumer sensors

## Reference checks

- compare SHT45 temperature and humidity against a known-good indoor reference meter
- compare BME680 pressure against a trusted local weather-station or indoor reference after adjusting for station differences
- record room conditions, placement, and time of day during every comparison

Reference checks should focus on repeatability and bias direction, not perfect agreement.

## Bench calibration

1. Let the assembled node stabilize indoors for at least 30 minutes after power-on.
2. Place the node beside a reference meter, not on top of it.
3. Record 15 to 30 minutes of readings at a fixed location.
4. Compute average offset for SHT45 temperature and humidity if a consistent bias appears.
5. Treat BME680 gas resistance as a baseline trend source; do not convert it to ppm without a validated model.
6. Repeat after moving the node to a second indoor environment with meaningfully different humidity or temperature.

If offsets are applied in firmware, keep them small, documented, and sensor-specific.

## Outdoor validation

Outdoor validation is optional for this subsystem and should only occur in sheltered locations. The purpose is to see how the prototype behaves under wider swings, not to certify it as a field-ready station. Capture:
- direct sun exposure or shade
- wind shielding
- proximity to walls, vents, grills, or vehicles
- condensation or splash risk

## Ongoing QA/QC

- rerun a short side-by-side check after any sensor replacement
- repeat reference comparisons quarterly for long-lived nodes
- inspect for drift after unusual heat, dust, or humidity exposure
- log any manual offsets in the node changelog or firmware config history

## Open questions

- How much offset correction should live in firmware versus downstream normalization?
- Should the project standardize on SHT45 as the temperature and humidity source across all nodes?
- At what point does a drifting BME680 get flagged for replacement instead of tolerated as a coarse trend sensor?
