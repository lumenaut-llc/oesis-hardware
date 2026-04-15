# Circuit Monitor v1.5

## Purpose

Define the bridge-stage current-draw monitoring node that uses split-core CT
clamps to observe HVAC and sump pump circuit activity at high fidelity.

## Minimum role

This node exists so the system can measure:

- HVAC operating state (off, fan only, compressor running, heating active)
- sump pump operating state (standby, starting, running, overload)
- power draw and cycle timing for both circuits
- equipment health via cycle duration and current-draw anomalies

## Why it belongs in `v1.5`

The circuit-monitor is the highest-fidelity implementation of the
equipment-state adapter concept. It provides `source_kind: "direct_measurement"`
data for house-state fields that the smoke, flood, and heat closed loops depend
on — without cloud API dependencies.

It sits at **Tier 3** of the tiered acquisition model for house-state fields:

- Tier 1: passive inference (thermal slope) — zero hardware, LOW confidence
- Tier 2: adapter integration (Ecobee/Nest API) — no new hardware, HIGH but
  cloud-dependent
- Tier 3: direct measurement (CT clamp) — highest fidelity, no cloud dependency

## First useful roles

- high-confidence HVAC mode for smoke loop (recirculate vs. fresh air matters)
- sump pump operational verification during flood events
- cycle duration tracking for equipment health and anomaly detection
- power draw baselines for outage-adjacent reasoning

## Hardware summary

- ESP32-S3 development board
- PZEM-004T (120V) or PZEM-016 (240V) via UART Modbus RTU
- SCT-013-030 split-core CT clamp (30A range)
- up to 2 monitored circuits per node

Full build guide, serial-JSON contract, firmware notes, and operator runbook are
in `../../circuit-monitor/`.

## Guardrails

- read-side monitoring only; this node does not actuate or control circuits
- CT clamp installation on panel circuits requires a qualified electrician
- for sump pumps on 120V outlets, the CT can clamp around the cord without
  panel access
- do not treat current-draw classification as a complete HVAC diagnostic —
  it provides operating state, not fault diagnosis

## Related

- `../../circuit-monitor/README.md` (full specification)
- `../equipment-state-adapter.md` (role definition)
- `../https://github.com/lumenaut-llc/oesis-program-specs/blob/main/contracts/v1.5/equipment-state-observation-schema.md`
- `../https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/node-taxonomy.md`
