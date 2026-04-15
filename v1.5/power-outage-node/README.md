# Power Outage Node v1.5

## Purpose

Define the bridge-stage power and continuity sensing node or adapter that lets
OESIS observe whether basic household function is being preserved during
disruption.

## Minimum role

This node exists so the system can measure:

- mains up or down state
- backup-power present or absent
- backup-power active or inactive

Richer battery, generator, and transfer posture can come later, but the minimum
continuity question belongs early because outage resilience is part of the
bridge from parcel sensing to household response.

## Why it belongs in `v1.5`

`power-outage-node` is an early resilience necessity, not a late-stage extra.

It helps the system answer:

- whether the house is still functionally powered
- whether a backup path exists
- whether outage conditions are changing the meaning of other parcel signals

## First useful roles

- continuity tracking during storms and outage-prone events
- backup-power posture for shelter and device-charging readiness
- support for later verification of what happened during a disruption window

## Placement posture

Place near the service or utility-entry context, or implement as a reliable
adapter into an equivalent local power-status source.

Preferred characteristics:

- stable power-state capture
- explicit distinction between mains loss and backup activation
- local continuity or buffering where practical
- serviceable installation and clear identity

## Guardrails

- do not treat this as a full energy-management platform
- do not imply generator or battery optimization logic already exists
- do not collapse outage sensing into a generic house-state note without a clear
  measured path

## Related

- `../../v1.0/README.md`
- `../https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/v1.5/house-state-and-verification-model.md`
- `../https://github.com/lumenaut-llc/oesis-program-specs/blob/main/contracts/v1.5/house-state-schema.md`
