# Circuit Monitor Node

## What it is

A non-invasive current-draw monitoring node that uses split-core CT clamps to observe HVAC and sump pump circuit activity. It reads AC current, voltage, power, and power factor via PZEM energy-monitoring modules and classifies equipment operating state from current-draw signatures.

## Taxonomy

This node is **v1.5 bridge hardware**, classified as an **equipment-state adapter** in the node taxonomy. See `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/node-taxonomy.md` for the full taxonomy and `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/version-and-promotion-matrix.md` for promotion rules.

## Why it matters

The circuit monitor provides high-fidelity HVAC mode, sump pump operational state, and power draw signatures without cloud API dependencies. It fills the equipment-state gap identified in the v1.5 bridge: the system can see outdoor conditions and indoor conditions, but cannot yet see what the house is doing in response. This node provides that missing layer by observing the electrical behavior of key equipment circuits directly.

## Standalone value

Even without the full parcel inference stack, this node gives a single parcel owner useful equipment evidence:

- equipment operating state (HVAC mode, sump pump activity)
- cycle timing (run duration, cycle count, duty cycle)
- power anomaly detection (overload, short cycling, failure to start)
- energy consumption tracking per monitored circuit
- baseline establishment for normal equipment behavior

## Scope for current version

- ESP32-S3 development board
- PZEM-004T for 120V circuits or PZEM-016 for 240V circuits
- split-core SCT-013 CT clamps (up to 30A)
- up to 2 monitored circuits (HVAC + sump pump)
- JSON packet output over serial and/or Wi-Fi
- Modbus RTU communication between ESP32 and PZEM modules

## Current maturity

Default posture: planned **v1.5 bridge hardware**, below `deployment maturity v1.0`.

This node is not yet in the accepted runnable slice. It exists in the specification layer as a planned equipment-state adapter. It becomes a deployment maturity candidate only when:

- the `equipment.circuit.snapshot` observation family is implemented in the ingest path
- the house-state and equipment-state-observation contracts in v1.5 are finalized
- a physical prototype has been validated against known HVAC and sump equipment

## Software implementation status

The observation family `equipment.circuit.snapshot` is **not yet implemented** in the ingest path. The serial JSON contract defined in this directory is a forward specification. Normalization, parcel-state integration, and downstream inference consumers do not yet exist for this packet family.

## How it connects to the larger system

The circuit monitor is an equipment-state evidence producer. Its responsibility is to publish trustworthy current-draw observations with inferred equipment state so downstream services can combine them with indoor conditions, outdoor conditions, and other evidence layers to model house response. In the v1.5 bridge, this node primarily informs HVAC mode reasoning and sump pump operational awareness while exercising the equipment-state observation path used by later action and outcome verification surfaces.

## Inputs

- AC current via CT clamp around the monitored circuit live wire
- AC voltage via PZEM terminal connections
- circuit identity from firmware configuration (e.g. "hvac_main", "sump_primary")
- local time or monotonic uptime from firmware
- device identity and firmware version
- optional Wi-Fi credentials if packets are forwarded upstream

## Outputs

- `current_a` -- RMS current draw in amperes
- `power_w` -- active power in watts
- `power_factor` -- power factor (0.0 to 1.0)
- `voltage_v` -- line voltage in volts
- `energy_kwh` -- cumulative energy consumption in kilowatt-hours
- `inferred_state` -- equipment state classification:
  - HVAC: `off`, `fan_only`, `compressor_running`, `heating_active`, `overload`, `unknown`
  - Sump: `standby`, `starting`, `running`, `overload`, `unknown`
- `cycle_duration_s` -- duration of current active cycle in seconds (null if no active cycle)
- `cycle_count` -- cumulative cycle count since last reset

## Risks and constraints

- Requires access to an electrical panel or outlet for CT clamp installation.
- CT clamp must go around the live wire only, never around the neutral, and never around both wires together. Clamping both wires cancels the magnetic field and reads zero.
- 240V circuits (central AC compressor, heat pump) need the appropriate PZEM variant (PZEM-016) and may require monitoring both legs of a split-phase circuit.
- Installation at the electrical panel should be performed by or supervised by a qualified electrician.
- For sump pumps on a 120V outlet, the CT clamp can be placed around the outlet cord without opening the panel.
- Current-draw classification thresholds vary by equipment make, model, and age. Factory defaults may need per-install calibration.
- This node should not overclaim equipment state; it provides evidence with uncertainty, not final equipment diagnostics.

## Dependencies

- house-state and equipment-state-observation contracts in v1.5
- `equipment.circuit.snapshot` observation family in the ingest path
- shared glossary
- basic ingest path on the software side
- serial JSON contract for first-build bring-up

## Parcel kit integration

This is an **optional equipment-state module**, not part of the default Tier 1 or Tier 2 parcel kit. It attaches when the parcel risk profile, operator capability, and use case justify circuit-level equipment monitoring. Typical attachment scenarios:

- smoke protection pilot where HVAC mode is needed for response verification
- flood-risk parcel where sump pump state is critical evidence
- energy and equipment health monitoring as a standalone value

## Required now

- PZEM module and CT clamp procurement path
- documented circuit identity mapping per install
- safety documentation for CT clamp installation
- known-good USB power and data path for the ESP32

## Add later

- enclosure rated for panel-adjacent or utility-room installation
- local buffering or logging path
- multi-circuit expansion beyond two channels
- integration with action-log and outcome-log surfaces

## Field-ready boundary

Do not describe this node as field-ready by default. It crosses into a deployment maturity claim only when the repo documents the field-hardening bundle for the specific install, including qualified electrician sign-off on CT clamp placement where panel access is involved.

## Serviceability notes

- keep the controller accessible for inspection and reflash
- use a physical node label once the node leaves pure bench use
- keep at least one spare PZEM module and CT clamp for any active field deployment
- verify CT clamp placement has not shifted after any panel service work

## Next milestones

- validate PZEM-004T Modbus communication on ESP32-S3
- establish current-draw signature baselines for common HVAC equipment
- implement `equipment.circuit.snapshot` observation family in the ingest path
- define house-state inference rules that consume circuit evidence
- publish versioned JSON packets to the ingest service
- define enclosure and mounting guidance for utility-room installation

## Open questions

See `open-questions.md` for unresolved decisions on default circuit count, calibration approach, defrost cycle detection, split-phase monitoring, and firmware-vs-downstream computation boundaries.

## Key docs

- `build-guide.md`
- `firmware-notes.md`
- `serial-json-contract.md`
- `operator-runbook.md`
- `open-questions.md`
- `firmware/README.md`
