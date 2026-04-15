# OESIS Hardware

Open hardware sensor nodes for the [Open Environmental Sensing and Inference System (OESIS)](https://github.com/lumenaut-llc/oesis-program-specs). Each node family produces structured JSON packets that feed into the OESIS inference pipeline to generate per-parcel environmental assessments.

## Node families

| Node | What it measures | Maturity | Lane |
|------|-----------------|----------|------|
| **[bench-air-node](bench-air-node/)** | Indoor air quality (temp, humidity, pressure, VOC trend) | Build guide + firmware; repeatable | v0.1+ |
| **[mast-lite](mast-lite/)** | Sheltered outdoor air (shared bench-air lineage) | Build guide exists; field hardening open | v0.2+ |
| **[flood-node](flood-node/)** | Low-point runoff water depth | Build guide exists; provisional calibration | v0.3+ |
| **[weather-pm-mast](weather-pm-mast/)** | Outdoor particulate matter and weather | Documented; not independently built | Future |
| **[thermal-pod](thermal-pod/)** | Scene-level thermal imaging (Pi + MLX90640) | R&D lane | Future |
| **[circuit-monitor](circuit-monitor/)** | Mains power and equipment state | v1.5 bridge hardware | Future |

## Parcel kit

The [parcel-kit](parcel-kit/) directory contains cross-node resources:

- [Integrated BOM](parcel-kit/integrated-parcel-kit-bom.md) — parts list by build tier
- [Power source guide](parcel-kit/power-source-guide.md) — power architecture for all node families
- [Field-hardening checklist](parcel-kit/field-hardening-checklist.md) — minimum posture for deployed nodes
- [Installation checklist](parcel-kit/parcel-installation-checklist.md) — parcel-level install steps
- [Procurement checklist](parcel-kit/parcel-kit-procurement-checklist.md) — sourcing guidance

## Getting started

**Build your first node:** Start with the [bench-air-node build guide](bench-air-node/build-guide.md). It requires an ESP32-S3 DevKitC-1, an SHT45, and a BME680 — all available from standard suppliers.

**Connect it to the runtime:**

```bash
# In the oesis-runtime repo
pip install -e ".[serial-bridge]"
python3 -m oesis.ingest.serve_ingest_api --host 127.0.0.1 --port 8787
python3 -m oesis.ingest.serial_bridge --serial-port /dev/cu.usbmodem101 --parcel-id parcel_demo_001
```

## Version lanes

Hardware docs follow the same lane model as the runtime:

| Lane | Hardware scope |
|------|---------------|
| **[v0.1](v0.1/)** | Baseline: bench-air-node only |
| **[v0.2](v0.2/)** | Two-node kit: bench-air + mast-lite |
| **[v0.3](v0.3/)** | Three-node kit: + flood-node |
| **[v1.0](v1.0/)** | Fielded parcel kit with trust scoring support |
| **[v1.5](v1.5/)** | Bridge hardware: indoor-response, power-outage, equipment-state |

## Sibling repositories

| Repository | What it contains |
|---|---|
| [oesis-program-specs](https://github.com/lumenaut-llc/oesis-program-specs) | Architecture, contracts, schemas, governance, release materials |
| [oesis-runtime](https://github.com/lumenaut-llc/oesis-runtime) | Python reference services (ingest, inference, parcel-platform) |
| [oesis-public-site](https://github.com/lumenaut-llc/oesis-public-site) | Public preview website |

## License

- **Hardware designs** (build guides, wiring, mechanical design, BOMs): [CERN Open Hardware Licence v2 — Strongly Reciprocal](LICENSE) (CERN-OHL-S-2.0)
- **Firmware** (code under `*/firmware/`): [GNU AGPL v3 or later](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/software/LICENSE)

See [NOTICE.md](NOTICE.md) for details.

## Contributing

Hardware contributions follow the same guidelines as the broader OESIS project. See the [contribution guide](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/CONTRIBUTING.md). Match your contribution's license to the asset class (hardware design → CERN-OHL-S-2.0, firmware → AGPL-3.0).
