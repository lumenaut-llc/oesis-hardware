# Field-Hardening Checklist

## Purpose

Define the minimum shared hardware and installation posture required before a node or parcel kit is described as deployed or field-ready.

## Governing rule

Field-hardening is part of the node.
It is not optional support gear once the docs use deployed language.

**Promotion context:** meeting this checklist is part of claiming **`deployment maturity v1.0`** for a node or kit. Program-phase **`v0.2`** (bench-air + mast-lite) requires that promotion story to be real, not aspirational — see `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/version-and-promotion-matrix.md` and `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/current/pre-1.0-version-progression.md`.

## Shared checklist

### Power and protection

- documented power source for the node (see `power-source-guide.md`)
- fuse or equivalent protected power posture where relevant
- reverse-polarity posture where relevant
- surge or transient posture for outdoor power entry

**Verification steps:**

- [ ] measured idle and peak current draw documented for the specific node build
- [ ] fuse or polyfuse rating matches peak draw with at least 50% margin
- [ ] power entry sealed against weather for outdoor nodes (IP67 cable gland or equivalent)
- [ ] cable routing avoids drip paths into enclosure (drip loop present for outdoor runs)
- [ ] power-loss behavior documented: does the node reboot cleanly, or is data at risk?
- [ ] USB adapter rating confirmed sufficient for peak draw (see power-source-guide.md for budgets)
- [ ] cable jacket rated for UV exposure if outdoor run is not inside conduit or under cover

### Local buffering and storage

- ring buffer, FRAM, microSD, SSD, or equivalent local history posture
- explicit decision for how recent evidence survives outages or reboots
- clean shutdown posture where the controller class needs it

### Wiring and connectors

- serviceable wiring, not loose long-term jumper posture
- connector strategy documented
- strain relief on enclosure cable entries
- drip-path aware routing for outdoor lanes

### Enclosure and mounting support

- cable glands or equivalent protected entries
- vent or membrane posture where airflow matters
- moisture posture, including desiccant or drain strategy where needed
- stable mount geometry appropriate to the node family

### Identity and service

- physical node label or QR label
- service access posture for reset, flash, or inspection
- visible status indicator if practical
- install notes tied back to the parcel record

### Spares and replacement

- one spare controller per active fielded family
- one spare primary sensing path per active fielded family
- documented replacement posture for the pilot field kit

## Node-family callouts

### `bench-air-node`

- fixed harness or equivalent stable wiring
- simple stable stand or enclosure posture
- label and service access are still required if deployed beyond the bench

### `mast-lite`

- protected outdoor power posture
- cable glands, venting, and connectorized leads
- sheltered placement discipline

### `weather-pm-mast`

- stable 5V PM power path
- airflow path that is deliberate and inspectable
- weather-interface posture before adding wind or rain channels

### `flood-node`

- rigid mount that preserves geometry
- dry zero reference
- field marker or staff gauge posture

### `thermal-pod`

- stable Pi power
- durable local storage
- clean shutdown posture
- repeatable field-of-view geometry

## Related docs

- `pilot-field-kit.md`
- `integrated-parcel-kit-bom.md`
- `parcel-installation-checklist.md`
- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/deployment-maturity-ladder.md`
