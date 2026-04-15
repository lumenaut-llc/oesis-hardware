# Open Questions

## Default circuit count

Should the default configuration monitor one circuit or two? A single-circuit default is simpler for first-build validation and avoids the PZEM address reprogramming step. A two-circuit default better represents the target use case (HVAC + sump) and exercises the multi-address Modbus path from the start.

## Threshold calibration strategy

What current-draw thresholds should be factory defaults versus per-install calibrated? Options:

- **Factory defaults only:** ship with the reference thresholds from the serial JSON contract and document that operators should adjust them. Risk: misclassification on equipment that does not match the reference range.
- **Per-install calibration pass:** require or encourage a calibration step during installation where the operator cycles the equipment through its modes and the firmware learns the actual current ranges. Risk: adds complexity and operator burden to installation.
- **Hybrid:** ship factory defaults with an optional calibration mode that can refine them. The firmware uses defaults until calibration data is available.

## Heat pump defrost cycle detection

Should the node attempt to distinguish heat pump defrost cycles from normal heating or cooling operation? During defrost, a heat pump temporarily reverses the refrigerant cycle, which can produce a distinctive current signature (compressor running + auxiliary heat strips). Detecting this would provide richer equipment-state evidence but adds classification complexity and may require equipment-specific threshold profiles.

## 240V split-phase monitoring

How should the node handle 240V split-phase circuits where both legs need monitoring? Options:

- **Single-leg monitoring:** measure one leg only and double the reading for approximate total power. Simpler hardware, less accurate if legs are unbalanced.
- **Dual-leg monitoring:** use two CT clamps and two PZEM channels for a single 240V circuit. More accurate but consumes both available monitoring channels for one circuit.
- **Single PZEM-016 with appropriate CT:** the PZEM-016 is designed for 240V circuits and may handle split-phase with a single CT on one leg depending on the installation topology.

## Firmware versus downstream computation

Should cycle duration and count be computed in firmware or left to downstream normalization? Arguments:

- **Firmware-side:** the firmware already tracks state transitions and has the timing precision needed for accurate cycle measurement. Downstream consumers get pre-computed values.
- **Downstream-side:** keeping the firmware as a dumb sensor reduces firmware complexity and lets downstream normalization apply consistent rules across different node types. The firmware only reports raw current/power and timestamps.
- **Hybrid (current spec):** firmware computes cycle state and duration for real-time local evidence, but downstream normalization can recompute from raw readings if needed for consistency.

## Energy counter reset policy

The PZEM module maintains a cumulative energy counter (`energy_kwh`) that persists across power cycles. Should the firmware reset this counter on boot, on a schedule, or leave it to the operator? A never-reset policy provides total lifetime energy but risks counter overflow. A periodic reset provides bounded measurement windows but loses continuity.

## Sump pump short-cycle detection

Sump pump short cycling (rapid on-off-on patterns) can indicate a stuck float switch, check valve failure, or overwhelming inflow. Should the firmware detect and flag short cycling as a distinct anomaly, or should this pattern be left to downstream analysis of cycle timing data?

## CT clamp accuracy at low currents

The SCT-013-030 (30A range) has relatively poor accuracy at very low currents (below 0.5A). This affects the precision of idle-state detection. Should the specification recommend a lower-range CT clamp (e.g. SCT-013-010, 10A range) for circuits with small idle currents, or is the 30A range sufficient given that the primary goal is state classification rather than precise energy metering?
