# Hardware v1.0

## Purpose

Describe the target-lane hardware posture without rewriting the current
`../v0.1/` hardware docs.

## Core stance

Hardware remains essential, but hardware is only one class of parcel evidence
surface.

The target lane should not be read as "add more weather hardware everywhere."
It should be read as:

- keep modular baseline nodes
- harden the first coherent indoor + sheltered-outdoor parcel kit
- treat several important next capabilities as non-node support surfaces

## Universal baseline nodes

- `bench-air-node`
- `mast-lite`

These remain the universal hardware baseline for the broader `v1.0` parcel-kit
lane.

## Geography-gated hazard modules

- `flood-node`
- `weather-pm-mast`
- `freeze-node`

These should attach by parcel context and hazard relevance rather than as a
universal default stack.

## Research-gated node

- `thermal-pod`

## Downstream support surfaces (staged after this lane)

- `building-and-site-metadata-surface`
- later `equipment-state-adapter`
- later `action-log`
- later `outcome-log` / response verification
- later `controls-compatibility-surface`

Bridge-stage node families such as `indoor-response-node` and
`power-outage-node` belong in `../v1.5/`, not in this `v1.0` lane.

## Design rule

Do not force every important next capability into a standalone physical node.
Some of the most important additions belong as adapters, metadata surfaces, and
logs that work beside the hardware fleet.

## Parcel-kit hardening target

The first strong `v1.0` hardware proof path is a field-credible two-node kit:

- `bench-air-node` + `mast-lite`
- one parcel binding and install metadata posture
- calibration and maintenance discipline that can support field use claims
- explicit deployment maturity labeling per node family
