# OESIS Hardware

Open hardware sensor nodes for the [Open Environmental Sensing and Inference System (OESIS)](https://github.com/lumenaut-llc/oesis-program-specs). Each node family produces structured JSON packets that feed into the OESIS inference pipeline to generate per-parcel environmental assessments.

## Repository layout

This repo is organized **version-first**: every version is a top-level directory, and node families live inside the version lane they belong to.

```
oesis-hardware/
├── v0.1/                   ← canonical baseline (node families, parcel-kit, calibration)
├── v0.2/ … v1.0/           ← promotion-marker lanes (inherit v0.1; document per-slice deltas)
└── v1.5/                   ← bridge-stage additive lane (circuit-monitor, indoor-response-node, …)
```

The canonical content for every baseline node family lives in [`v0.1/`](v0.1/). Later lanes (`v0.2/`–`v1.0/`) are lane-index manifestos that inherit v0.1 and name what's new at that slice. The `v1.5/` lane adds bridge-stage hardware that does not exist in v0.1.

## Node families (canonical baseline)

| Node | What it measures | Lane introduced |
|------|------------------|-----------------|
| [bench-air-node](v0.1/bench-air-node/) | Indoor air quality (temp, humidity, pressure, VOC trend) | v0.1 |
| [mast-lite](v0.1/mast-lite/) | Sheltered outdoor air (shared bench-air lineage) | v0.1 (target v0.2) |
| [flood-node](v0.1/flood-node/) | Low-point runoff water depth | v0.1 (target v0.3) |
| [weather-pm-mast](v0.1/weather-pm-mast/) | Outdoor particulate matter and weather | v0.1 (future) |
| [thermal-pod](v0.1/thermal-pod/) | Scene-level thermal imaging (Pi + MLX90640) | v0.1 (R&D) |
| [circuit-monitor](v1.5/circuit-monitor/) | Mains power and equipment state | v1.5 bridge |
| [indoor-response-node](v1.5/indoor-response-node/) | House-state feedback | v1.5 bridge |
| [power-outage-node](v1.5/power-outage-node/) | Outage detection | v1.5 bridge |

## Parcel kit (baseline)

[`v0.1/parcel-kit/`](v0.1/parcel-kit/) contains cross-node resources:

- [Integrated BOM](v0.1/parcel-kit/integrated-parcel-kit-bom.md) — parts list by build tier
- [Power source guide](v0.1/parcel-kit/power-source-guide.md) — power architecture for all node families
- [Field-hardening checklist](v0.1/parcel-kit/field-hardening-checklist.md) — minimum posture for deployed nodes
- [Installation checklist](v0.1/parcel-kit/parcel-installation-checklist.md) — parcel-level install steps
- [Procurement checklist](v0.1/parcel-kit/parcel-kit-procurement-checklist.md) — sourcing guidance

## Calibration

[`v0.1/calibration/`](v0.1/calibration/) documents node-family-agnostic calibration posture. Per-family calibration procedures live inside each node's directory.

## Getting started

**Build your first node:** Start with the [bench-air-node build guide](v0.1/bench-air-node/build-guide.md). It requires an ESP32-S3 DevKitC-1, an SHT45, and a BME680 — all available from standard suppliers.

**Connect it to the runtime:**

```bash
# In the oesis-runtime repo
pip install -e ".[serial-bridge]"
python3 -m oesis.ingest.serve_ingest_api --host 127.0.0.1 --port 8787
python3 -m oesis.ingest.serial_bridge --serial-port /dev/cu.usbmodem101 --parcel-id parcel_demo_001
```

## Version lanes

| Lane | Hardware scope |
|------|---------------|
| [v0.1](v0.1/) | Canonical baseline: all primary node families, parcel-kit, calibration |
| [v0.2](v0.2/) | Two-node kit sign-off: bench-air + mast-lite |
| [v0.3](v0.3/) | Three-node kit sign-off: + flood-node |
| [v0.4](v0.4/) | Registry lifecycle + install metadata |
| [v0.5](v0.5/) | Governance enforcement (software slice; no new hardware) |
| [v1.0](v1.0/) | Fielded parcel-kit target |
| [v1.5](v1.5/) | Bridge hardware: circuit-monitor, indoor-response, power-outage, equipment-state |

## Calibration posture and admissibility

Every node family in this repository is subject to the calibration program and adapter-trust program defined in [`oesis-program-specs/architecture/system/`](https://github.com/lumenaut-llc/oesis-program-specs/tree/main/architecture/system):

- **Physical sensor nodes** (bench-air, mast-lite, flood, weather-pm-mast, thermal-pod): governed by [`calibration-program.md`](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/calibration-program.md). Each node's build spec declares a §F metadata block (deployment class, deployment-maturity target, reference instruments, burn-in parameters, protective fixtures). Readings that fail the §C admissibility check do not enter coefficient fitting or parcel-state claims.
- **Adapter-derived nodes and methods** (circuit-monitor, future equipment-state-adapter, passive thermal-slope inference): governed by [`adapter-trust-program.md`](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/adapter-trust-program.md). Declared §F metadata block covers source authority, pinned API contract version, onboarding gate, cross-check posture.

Deployment-class standards (power tier, enclosure IP rating, transport tier per class) live in [`deployment-maturity-ladder.md`](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/deployment-maturity-ladder.md); sensor variant selection principles live in [`sensor-placement-and-representativeness-guide.md`](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/sensor-placement-and-representativeness-guide.md).

A node family's inclusion in a promoted lane (v0.2, v0.3, v1.0, …) requires it to meet the calibration posture named in the lane's promotion bar per [`pre-1.0-version-progression.md`](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/current/pre-1.0-version-progression.md) item 5.

## Sibling repositories

| Repository | What it contains |
|---|---|
| [oesis-program-specs](https://github.com/lumenaut-llc/oesis-program-specs) | Architecture, contracts, schemas, governance, release materials |
| [oesis-runtime](https://github.com/lumenaut-llc/oesis-runtime) | Python reference services (ingest, inference, parcel-platform) |
| [oesis-contracts](https://github.com/lumenaut-llc/oesis-contracts) | JSON Schemas, examples, contract bundles |
| [oesis-builds](https://github.com/lumenaut-llc/oesis-builds) | Build vault — per-unit specs, procedures, references |
| [oesis-public-site](https://github.com/lumenaut-llc/oesis-public-site) | Public preview website |

## License

- **Hardware designs** (build guides, wiring, mechanical design, BOMs): [CERN Open Hardware Licence v2 — Strongly Reciprocal](LICENSE) (CERN-OHL-S-2.0)
- **Firmware** (code under `*/firmware/`): [GNU AGPL v3 or later](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/software/LICENSE)

See [NOTICE.md](NOTICE.md) for details.

## Contributing

Hardware contributions follow the same guidelines as the broader OESIS project. See the [contribution guide](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/CONTRIBUTING.md). Match your contribution's license to the asset class (hardware design → CERN-OHL-S-2.0, firmware → AGPL-3.0).
