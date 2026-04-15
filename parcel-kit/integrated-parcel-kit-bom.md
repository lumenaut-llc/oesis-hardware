# Integrated Parcel Kit BOM

## Purpose

Translate the integrated parcel system design into a practical first bill of materials by deployment tier.

Companion checklist:

- `parcel-kit-procurement-checklist.md`
- `parcel-installation-checklist.md`

Tier labels here are **build tiers**, not automatic proof of program-phase promotion. See `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/version-and-promotion-matrix.md` (current truth vs next promotion) and `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/node-taxonomy.md`.

## Governing rule

The parcel kit is one system, but not one physical box.
Use specialized nodes where placement requirements differ, then unify them through one parcel identity, one node registry, and one software path.

This guide also follows the deployment maturity overlay:

- `deployment maturity v0.1` for bench prototypes and first bring-up
- `deployment maturity v1.0` for the first field-hardened parcel kit
- `deployment maturity v1.5` for stronger device-operations and trust hardening

## Shared non-hardware system parts

Every tier assumes these system parts exist:

- one `parcel_id`
- one parcel-context record
- one node-registry record
- one sharing-settings record
- one ingest endpoint path
- one local validation path through `make oesis-check`

## Shared field-hardening bundle

No node should be described as deployed or field-ready unless the active parcel kit also includes a documented answer for these categories:

| Category | Required posture |
| --- | --- |
| power and protection | protected power entry, polarity discipline, and outdoor transient posture where relevant |
| local buffering and storage | ring buffer, FRAM, microSD, or other durable local history posture |
| serviceable wiring | connectorized or otherwise stable wiring, strain relief, and cable routing |
| enclosure support | cable glands, venting, moisture posture, and corrosion-aware hardware where relevant |
| identity and service | physical label, service access posture, and visible service status if practical |
| spares and replacement | one spare controller and one spare sensing path for active fielded families |

## Tier 1: Fastest useful parcel kit

### Goal

Deliver useful single-home value on the shortest timeline.
This is primarily a `deployment maturity v0.1` lane.

### Hardware BOM

| Quantity | Item | Notes |
| --- | --- | --- |
| 1 | `bench-air-node` build | indoor or sheltered reference node |
| 1 | ESP32-S3 development board | inside bench-air-node |
| 1 | SHT45 breakout | primary temperature and humidity |
| 1 | BME680 breakout | pressure and gas-trend support |
| 1 | USB power supply and cable | simplest first power path |

### Software and ops BOM

| Quantity | Item | Notes |
| --- | --- | --- |
| 1 | ingest service path | serial validation first, HTTPS push later |
| 1 | inference engine path | uses local plus public evidence |
| 1 | parcel-platform view | one dwelling-facing parcel surface |

### When to choose it

- fastest pilot
- indoor smoke and heat focus
- minimal hardware risk
- strong fit for bench proof and software validation before field-hardening the parcel kit

## Tier 2: First integrated parcel kit

### Goal

Add outdoor parcel context without waiting for the richer mast build.
This is the first honest `deployment maturity v1.0` target.

### Additional hardware BOM

| Quantity | Item | Notes |
| --- | --- | --- |
| 1 | `mast-lite` build | sheltered outdoor reference node |
| 1 | second ESP32-S3 development board | separate outdoor controller |
| 1 | second SHT45 breakout | outdoor primary temperature and humidity |
| 1 | second BME680 breakout | outdoor pressure and gas-trend support |
| 1 | vented enclosure and shield hardware | preserve airflow and survivability |
| 1 | basic mount or eave hardware | stabilize sheltered outdoor placement |
| 2 | local buffering or storage parts | one answer per active controller before calling the parcel kit deployed |
| 1 set | protected power and connector parts | fuse, polarity, terminal, and connector posture for active lanes |
| 1 set | cable glands, venting, and strain relief parts | part of the node, not afterthought support gear |
| 2 | physical node labels | required for install and service discipline |

### Integration requirements

| Quantity | Item | Notes |
| --- | --- | --- |
| 1 | parcel-scoped node registry | binds indoor and outdoor nodes to one parcel |
| 2 | stable node IDs | one per controller |
| 1 | shared transport posture | recommended `https_push` for both nodes |

### When to choose it

- first full parcel operator pilot
- need indoor versus outdoor distinction
- want one coherent parcel kit without PM mast complexity

## Optional hazard module: Flood

### Add only when

- the parcel has a meaningful runoff low point
- flood and drainage are an actual target use case

### Additional hardware BOM

| Quantity | Item | Notes |
| --- | --- | --- |
| 1 | `flood-node` build | low-point runoff evidence module |
| 1 | ESP32-S3 development board | separate flood controller |
| 1 | MB7389 ultrasonic range sensor | low-point depth evidence |
| 1 | weather-resistant enclosure | splash-aware housing |
| 1 | fixed mounting bracket | must preserve repeatable geometry |
| 1 | field marker or staff gauge | required for honest geometry and repeatable reference checks |

### Additional integration requirements

| Quantity | Item | Notes |
| --- | --- | --- |
| 1 | runoff low-point install record | part of parcel context and registry meaning |
| 1 | dry-reference measurement | required before stronger interpretation |
| 1 | conservative flood observation family | keep point evidence distinct from parcel-wide claims |
| 1 | geometry-stability check | do not treat the node as stronger parcel evidence until mount drift is controlled |

## Tier 3: Rich outdoor upgrade

### Goal

Promote the outdoor leg from `mast-lite` to `weather-pm-mast` after the simpler kit is stable.
Treat this as a `deployment maturity v1.5` target unless the PM and weather support hardware are already documented and provisioned.

### Additional hardware BOM

| Quantity | Item | Notes |
| --- | --- | --- |
| 1 | `weather-pm-mast` build | richer outdoor evidence path |
| 1 | SPS30 | particulate sensing |
| 1 | mast hardware and enclosure upgrades | exposure-ready mounting |
| 1 | stable 5V PM power path | required for honest PM-first field behavior |
| 1 | interface posture for wind and rain hardware | RJ11 or equivalent wiring discipline when those lanes are added |
| optional | wind and rain components | only after PM-first stability |

### Replacement rule

Treat `weather-pm-mast` as an upgrade to the outdoor reference lane, not a requirement before the first integrated pilot.

## Separate R&D lane: Thermal

### Keep separate because

- controller class differs
- privacy review differs
- observation family differs

### Hardware BOM

| Quantity | Item | Notes |
| --- | --- | --- |
| 1 | `thermal-pod` build | fixed-scene thermal system |
| 1 | Raspberry Pi 5 | current controller |
| 1 | MLX90640 | thermal array |
| 1 | hooded enclosure | must control scene and self-heating |

### Product rule

Do not fold the thermal pod into the default integrated parcel kit until the derived-only contract and usefulness case are stronger.

## Node-to-system map

| Node | Parcel role | Normalized family target | Default status |
| --- | --- | --- | --- |
| `bench-air-node` | indoor reference | `air.node.snapshot` | required |
| `mast-lite` | outdoor reference | `air.node.snapshot` | recommended |
| `flood-node` | runoff low point | `flood.low_point.snapshot` | optional |
| `weather-pm-mast` | richer outdoor upgrade | `air.pm_weather.snapshot` | second wave |
| `thermal-pod` | scene-level thermal R&D | `thermal.scene.snapshot` | separate lane |

## Recommended first integrated pilot bundle

- 1 `bench-air-node`
- 1 `mast-lite`
- 1 parcel node-registry record
- 1 shared `https_push` transport posture
- 1 shared field-hardening bundle covering power, buffering, connectors, enclosure support, labels, and spares
- optional `flood-node` only on the right parcels

That is the best balance between singular system design, truthful hazard coverage, and timeline pressure.
