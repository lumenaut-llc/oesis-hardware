# Build Guide

## Summary

Build a circuit monitoring node around an ESP32-S3 with one or two PZEM energy monitoring modules and split-core CT clamps to observe HVAC and sump pump circuit activity. The first-build goal is a stable bench prototype that proves PZEM Modbus communication, repeatable current/power reads, and equipment state classification before any enclosure or field installation complexity is added.

## Current maturity

Default posture: planned **v1.5 bridge hardware**, below `deployment maturity v1.0`.

Use this guide as the bench and first-bring-up path. Do not read it as a field-ready guide. Field installation involving electrical panel access requires a qualified electrician.

**Program posture:** this is a planned **v1.5 bridge** equipment-state adapter. It is not part of the current-truth hardware baseline for program-phase `v0.1`. See `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/node-taxonomy.md`.

## What this build actually proves

This build proves the minimum equipment-state evidence path:

- stable PZEM Modbus RTU communication
- repeatable current, voltage, and power reads
- current-draw based equipment state classification
- cycle detection and timing
- first end-to-end equipment observation packet emission

It does **not** yet prove:

- integration with the ingest path (observation family not yet implemented)
- house-state inference from circuit evidence
- action-to-outcome verification loops
- field-grade installation or enclosure

## Required tools

- soldering iron with fine tip (for header work only, not mains wiring)
- solder
- flush cutters and small pliers
- breadboard or small perfboard
- multimeter with AC current and voltage capability
- USB-C cable for programming and power
- wire strippers

## Required materials

- 1 ESP32-S3 DevKitC-1 with 3.3 V logic
- 1 or 2 PZEM-004T v3.0 modules (for 120V circuits) or PZEM-016 (for 240V circuits)
- 1 or 2 SCT-013-030 split-core CT clamps (30A, 1V output)
- jumper wires for UART connections
- breadboard or perfboard and headers
- USB-C power supply for the ESP32
- optional: small project enclosure for utility room mounting

## Safety

**Read this section completely before starting.**

- **Do not open an electrical panel unless you are a qualified electrician or are working under the direct supervision of one.** Exposed bus bars inside a panel carry lethal voltage.
- Split-core CT clamps are designed to clamp around a wire without disconnecting it. This is their primary safety advantage. Never use solid-core CTs that require threading a wire through them.
- The CT clamp must go around the **live (hot) wire only**. Never clamp around the neutral wire alone. Never clamp around both wires together (the magnetic fields cancel and the reading will be near zero).
- For sump pumps on a standard 120V outlet, the CT clamp can be placed around the outlet cord without any panel access. Use a cord splitter or extension cord that separates the hot and neutral conductors to allow clamping the hot wire individually.
- The PZEM voltage measurement terminals connect to line voltage. Keep these connections secure and insulated. Use appropriate wire gauge and terminal covers.
- The ESP32 and low-voltage side must be kept physically separated from any mains-voltage wiring. Do not route mains wires near the ESP32 board.
- Do not use this prototype as a life-safety device or equipment protection relay.
- Power the ESP32 only from a known-good USB source, not from the monitored circuit.

## Preflight

1. Confirm you have a known-good USB data cable, not a power-only cable.
2. Confirm the ESP32 board powers on and appears in the host flashing tool before adding PZEM modules.
3. If using two PZEM modules, reprogram the second module to Modbus address `0x02` before connecting it to the shared UART bus. Connect only that module, use the address-change sketch, then disconnect.
4. Inspect CT clamp connectors and PZEM terminal screws for damage.

## Assembly steps

### Step 1: ESP32 bench setup

1. Seat the ESP32-S3 on the breadboard.
2. Flash a basic serial test sketch to confirm the board is working.
3. Power off.

### Step 2: Single PZEM module wiring

1. Connect the first PZEM-004T to the ESP32:
   - PZEM TX to ESP32 `GPIO16` (UART2 RX)
   - PZEM RX to ESP32 `GPIO17` (UART2 TX)
   - PZEM 5V from a suitable source (PZEM-004T has its own power from AC mains, not from the ESP32 3.3V rail)
2. Connect the SCT-013 CT clamp to the PZEM current input terminals.
3. Connect the PZEM voltage measurement wires to a known AC source (e.g. a bench outlet via a properly fused test cord).

### Step 3: First PZEM validation

1. Flash the PZEM test sketch.
2. Open the serial monitor at `115200`.
3. Confirm voltage, current, and power readings appear.
4. Clamp the CT around a known load (e.g. a desk lamp or fan) and verify nonzero current and power readings.
5. Remove the load and confirm current drops to near zero.

### Step 4: Second PZEM module (if using two circuits)

1. Power off.
2. Confirm the second PZEM module has been reprogrammed to address `0x02`.
3. Connect the second PZEM module to the same UART bus (shared TX/RX lines with the first module).
4. Connect the second CT clamp to the second PZEM current input.
5. Power on and confirm both addresses respond in the test sketch.

### Step 5: Flash circuit monitor firmware

1. Flash the circuit monitor serial JSON sketch.
2. Open the serial monitor at `115200`.
3. Confirm JSON packets appear with data from all connected circuits.
4. Verify `inferred_state` changes when load is applied and removed from the CT clamp.

## Wiring notes

- PZEM TX/RX are 5V TTL level. Most ESP32-S3 GPIO pins are 5V tolerant on input, but confirm your specific board. A level shifter may be needed for reliable long-term operation.
- Both PZEM modules share the same UART2 bus. Modbus addresses differentiate them.
- Keep UART wiring short (under 30 cm) to avoid Modbus communication errors.
- The PZEM-004T powers itself from the AC mains connection. It does not need power from the ESP32.
- CT clamp polarity matters for power factor sign but not for current magnitude. If power factor reads negative, reverse the CT clamp orientation.

## Firmware setup

1. Assign a stable `node_id` such as `circuit-monitor-01`.
2. Configure circuit identities in the firmware config (e.g. `hvac_main`, `sump_primary`).
3. Set current-draw classification thresholds appropriate for the target equipment.
4. Start with serial logging before enabling any Wi-Fi transport.
5. Use `GPIO16` for UART2 RX and `GPIO17` for UART2 TX in the firmware configuration.
6. Enable serial JSON output first.

The exact first-build serial payload target is defined in `serial-json-contract.md`.

## First test procedure

1. With one PZEM module connected, boot the node and confirm Modbus communication succeeds.
2. With no load on the CT clamp, verify current reads near zero and `inferred_state` is `off` or `standby`.
3. Apply a known load (desk lamp, fan, or space heater) and verify current, power, and state classification change appropriately.
4. Remove the load and verify the node detects the transition back to idle and reports `cycle_active: false`.
5. If using two PZEM modules, repeat steps 2-4 for the second circuit.
6. Leave the node running for 30 to 60 minutes monitoring a real load and inspect cycle detection, packet continuity, and any Modbus communication errors.

## Acceptance criteria

- PZEM module at address `0x01` responds to Modbus queries
- if configured, PZEM module at address `0x02` also responds
- voltage reads within 5% of known line voltage
- current reads near zero with no load on the CT clamp
- current reads plausibly with a known load applied
- `inferred_state` transitions match applied and removed loads
- cycle detection correctly tracks start and end of load application
- serial JSON output matches the contract in `serial-json-contract.md`

## Maintenance

- inspect CT clamp placement if readings drift or become erratic
- verify PZEM module Modbus communication after firmware changes
- check that CT clamp jaw is fully closed and seated around the wire
- revalidate thresholds if monitored equipment is replaced or serviced

## Links to related docs

- `README.md`
- `wiring.md`
- `calibration.md`
- `firmware-notes.md`
- `serial-json-contract.md`
- `operator-runbook.md`
- `open-questions.md`
- `firmware/README.md`
