# Calibration

## Goals

- verify that the PM-first mast is stable enough to trust as parcel evidence input
- separate real particulate and weather changes from siting, contamination, or enclosure artifacts
- document maintenance and cleaning assumptions explicitly

## Reference checks

- compare SHT45 temperature and humidity against a known-good sheltered outdoor reference
- compare BME680 pressure against a trusted local weather reference
- compare SPS30 PM trends against a trusted local PM reference if one exists
- record wind, splash, and nearby source conditions during every comparison

## Bench calibration

1. Validate the environmental stack first.
2. Add the PM sensor and confirm packet continuity on the bench.
3. Confirm that PM values move plausibly rather than remaining fixed or obviously corrupted.

## Outdoor validation

1. Mount the node in its first mast position.
2. Let the node stabilize for at least 30 minutes.
3. Record 30 to 60 minutes of readings at a fixed location.
4. Note any splash, dust, or direct-source events that might explain PM spikes.
5. Inspect airflow paths and contamination risk after the first outdoor run.

## Ongoing QA/QC

- inspect and clean particulate pathways on a defined cadence
- rerun a short side-by-side check after enclosure or mounting changes
- document service and cleaning history for the PM sensor

## Open questions

- How much PM smoothing belongs in firmware versus downstream normalization?
- What cleaning interval should be considered normal for the SPS30 in this deployment style?
- When should wind and rain be merged into the same hardware track instead of staying optional?
