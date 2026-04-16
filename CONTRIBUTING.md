# Contributing to oesis-hardware

Thank you for your interest in contributing to the OESIS open hardware sensor
network. This document covers hardware-specific contribution guidelines. For
project-wide governance and process, see the
[CONTRIBUTING.md](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/CONTRIBUTING.md)
in oesis-program-specs.

## Two contribution tracks

This repository contains two kinds of assets under different licenses:

| Track | Assets | License | Examples |
|-------|--------|---------|----------|
| Hardware design | Build guides, wiring diagrams, mechanical design, BOMs | CERN-OHL-S-2.0 | `build-guide.md`, `wiring.md`, `calibration.md` |
| Firmware code | Arduino sketches, Python scripts, helper tools | AGPL-3.0+ | `firmware/src/*.cpp`, `*.ino`, `*.py` |

**Match your contribution's license to the asset class.** If you are adding a
build guide, it falls under CERN-OHL-S-2.0. If you are adding firmware code, it
falls under AGPL-3.0+.

## Node family structure

Every node family directory follows a standard structure. If you are creating a
new node or adding to an existing one, maintain this layout:

```
<node-family>/
├── README.md                  Purpose, scope, maturity, inputs/outputs, risks
├── build-guide.md             Step-by-step assembly instructions
├── wiring.md                  GPIO/I2C pin assignments, hardware connections
├── calibration.md             Sensor calibration procedures
├── firmware-notes.md          Firmware design, sampling cadence, packet shape
├── serial-json-contract.md    JSON packet schema and field definitions
├── operator-runbook.md        Deployment, troubleshooting, field operations
├── open-questions.md          Known limitations and future work
└── firmware/
    ├── platformio.ini         PlatformIO project configuration
    ├── src/                   C++ source files
    ├── secrets.example.h      Template for Wi-Fi/config credentials
    └── tools/                 Helper scripts (serial capture, etc.)
```

## Where to contribute

Check the node maturity table in the [README](README.md) before starting work.
Nodes at different maturity levels need different kinds of contributions:

| Maturity | What is most useful |
|----------|-------------------|
| **Build guide + firmware; repeatable** (bench-air) | Field testing, calibration refinement, documentation improvements |
| **Build guide exists; field hardening open** (mast-lite, flood) | Field deployment reports, calibration data, edge-case firmware fixes |
| **Documented; not independently built** (weather-PM) | Independent build attempts and build-guide feedback |
| **R&D / early stage** (thermal-pod, circuit-monitor) | Design discussion in issues before submitting PRs |

## Firmware guidelines

### Arduino/PlatformIO nodes

- Target framework: Arduino on ESP32 via PlatformIO
- Pin dependency versions in `platformio.ini` (e.g., `espressif32@6.7.0`)
- Use `secrets.example.h` as a template — never commit actual credentials
- Test serial JSON output against the node's `serial-json-contract.md`
- Include a `tools/capture_serial.sh` or equivalent for validation

### Python nodes (thermal-pod)

- Python 3.9+ with type hints
- Use `config.example.json` for configuration templates
- Include `requirements.txt` with pinned versions
- Follow the existing style: modern annotations (`list[float]`, `str | None`)

### General firmware rules

- Every firmware change must produce output that matches the node's serial-JSON
  contract. If the packet shape changes, update the contract document first.
- Never commit `secrets.h`, `config.h`, or any file containing credentials.
  Use `.example` suffixes for templates.
- Document sampling cadence, power modes, and error handling in
  `firmware-notes.md`.

## Documentation guidelines

- Every node README must include a **Risks and constraints** section
- Build guides must be reproducible by someone with the listed BOM and tools
- Calibration documents must specify the reference instrument and procedure
- Operator runbooks must cover: installation, normal operation, troubleshooting,
  decommissioning
- Use tables for reference material, code blocks with language tags for examples

## Cross-repo coordination

Hardware nodes produce serial JSON packets that the
[oesis-runtime](https://github.com/lumenaut-llc/oesis-runtime) ingest layer
consumes. When changing a node's packet format:

1. Update the `serial-json-contract.md` in this repo
2. Update the corresponding schema in
   [oesis-program-specs](https://github.com/lumenaut-llc/oesis-program-specs)
3. Update the normalizer in oesis-runtime
4. Run `make cross-repo-sync` from oesis-program-specs to verify alignment

See the
[cross-repo architecture](https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/cross-repo-architecture.md)
for the full system data flow.

## Submitting changes

1. Fork the repository and create a feature branch
2. Make your changes following the guidelines above
3. Ensure no secrets or credentials are included
4. Open a pull request with:
   - What node family is affected
   - What the change does and why
   - How you tested it (bench test, field deployment, etc.)
5. Reference any related issues or PRs in sibling repos

## Questions?

Open an issue in this repository for hardware-specific questions. For
project-wide questions, use
[oesis-program-specs issues](https://github.com/lumenaut-llc/oesis-program-specs/issues).
