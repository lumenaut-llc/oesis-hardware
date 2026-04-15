# Equipment State Adapter v1.5

## Purpose

Define the bridge-stage adapter surface that captures the operating posture of
relevant household systems without pretending OESIS is already a controls
platform.

## Minimum role

This adapter should capture read-side state where relevant, such as:

- HVAC mode
- fan state
- recirculation versus fresh-air state
- purifier state
- window or shade state
- sump or drain equipment state

The point is to learn what operating surfaces exist at a parcel and what state
they were in when outcomes changed.

## Why it belongs in `v1.5`

Later bounded recommendations and controls cannot be layered in coherently if
the system never learns what operational surfaces even exist at a parcel.

This adapter is therefore part of the minimum bridge for:

`hazard -> house state -> action -> measured outcome`

## Implementation posture

This is primarily an adapter and support surface, not necessarily a standalone
hardware node.

Possible sources may include:

- local thermostat state
- purifier status indicators
- shade or window status feeds
- pump or sump controller state
- manual operator capture when no direct integration exists yet

## Guardrails

- read-side state is the priority; full compatibility inventory belongs later
- do not imply that knowing state means it is safe to actuate
- do not collapse coarse observed state into a claim of full automation
- preserve whether the source is direct measurement, adapter-derived, or manual
  entry

## Related

- `../https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/v1.5/house-state-and-verification-model.md`
- `../https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/node-taxonomy.md`
- `../https://github.com/lumenaut-llc/oesis-program-specs/blob/main/contracts/v1.5/intervention-event-schema.md`
