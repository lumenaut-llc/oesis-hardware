# Indoor Response Node v1.5

## Purpose

Define the bridge-stage indoor sensing node that lets OESIS observe whether the
house is actually buffering occupants from outdoor conditions.

## Minimum role

This node exists so the system can measure:

- indoor PM2.5
- indoor temperature
- indoor relative humidity

Without this node, the product can describe outdoor hazard and parcel condition
but cannot honestly judge whether the indoor environment is protecting people.

## Why it belongs in `v1.5`

`indoor-response-node` is one of the minimum bridge surfaces required for:

`hazard -> house state -> action -> measured outcome`

It is the key measurement path for the first serious smoke and heat response
loops.

## First serious proof path

Smoke protection remains the clearest first proof:

- outdoor evidence from `mast-lite` or similar parcel-edge sensing
- indoor PM2.5 / temperature / RH from this node
- observed equipment posture where available
- recorded household action
- measured indoor improvement over a bounded response window

## Placement posture

Install in indoor occupied or critical zones rather than treating this node as
a parcel-edge or outdoor reference device.

Preferred characteristics:

- stable indoor placement
- useful occupant exposure relevance
- serviceable calibration and maintenance posture
- power and buffering suitable for continuity during important events

## Guardrails

- do not treat this node as a substitute for the outdoor reference path
- do not infer whole-house truth from one badly placed indoor sensor
- do not describe the node as a controls surface
- do not overclaim parcel safety from sparse indoor evidence alone

## Related

- `../../v1.0/README.md`
- `../https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/v1.5/house-state-and-verification-model.md`
- `../https://github.com/lumenaut-llc/oesis-program-specs/blob/main/contracts/v1.5/house-state-schema.md`
