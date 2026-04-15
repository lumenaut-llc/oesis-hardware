# Hardware v1.5

## Purpose

Hold the bridge-stage hardware posture for the measurement-to-intervention
foundation.

## Minimum bridge hardware and adapters

- `indoor-response-node`
- `power-outage-node`
- `equipment-state-adapter`
- `circuit-monitor`

These are the minimum additional physical or adapter surfaces needed to connect
parcel conditions to house response.

## Guardrail

Do not read this lane as "add more hardware everywhere."
Several important `v1.5` additions remain support surfaces rather than new
standalone nodes.

## Initial contents

- `indoor-response-node/README.md`
- `power-outage-node/README.md`
- `equipment-state-adapter.md`
- `circuit-monitor/README.md`

## Design rule

The purpose of this lane is to document the minimum bridge from parcel sensing
to household response measurement:

`hazard -> house state -> action -> measured outcome`

That means the hardware story here should emphasize the minimum observability
needed for closed-loop reasoning, not a broad expansion of outdoor weather
instrumentation.
