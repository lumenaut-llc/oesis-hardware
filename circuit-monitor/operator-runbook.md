# Operator Runbook

## Purpose

Provide the shortest repeatable path from freshly wired circuit monitor hardware to a locally validated equipment-state observation packet.

## What this runbook proves

This runbook proves the minimum equipment-state evidence path:

- the circuit monitor node emits a valid `oesis.circuit-monitor.v1` packet
- PZEM Modbus communication is stable
- current-draw classification produces correct equipment states for known loads
- cycle detection tracks transitions between idle and active states

It does **not** prove the later integration surfaces yet:

- ingest path normalization for `equipment.circuit.snapshot`
- house-state inference from circuit evidence
- action-to-outcome verification
- indoor conditions correlation with equipment state

## Before you start

- ESP32-S3 DevKitC-1 wired with `GPIO16` as UART2 RX and `GPIO17` as UART2 TX
- at least one PZEM-004T (120V) or PZEM-016 (240V) module connected
- SCT-013-030 CT clamp connected to the PZEM current input terminals
- PZEM voltage terminals connected to a known AC source
- Arduino IDE, PlatformIO, or equivalent ESP32 workflow installed
- if using two PZEM modules, the second module must already be reprogrammed to Modbus address `0x02`

## Stage 1: verify PZEM communication

1. Flash the PZEM Modbus test sketch
2. Open the serial monitor at `115200`
3. Confirm you see voltage and current readings from address `0x01`

Expected output shape:

```text
PZEM address 0x01:
  voltage: 121.3 V
  current: 0.02 A
  power: 0.4 W
  energy: 0.00 kWh
  frequency: 60.0 Hz
  power_factor: 0.08
```

Stop here if no readings appear. Check UART wiring (`GPIO16`/`GPIO17`), PZEM power (AC mains connected), and Modbus address.

## Stage 2: verify second PZEM module (if configured)

1. With both modules connected to the shared UART bus, reflash or reboot with the test sketch
2. Confirm readings appear for both address `0x01` and address `0x02`

Expected output shape:

```text
PZEM address 0x01:
  voltage: 121.3 V
  current: 0.02 A
  ...
PZEM address 0x02:
  voltage: 121.4 V
  current: 0.01 A
  ...
```

Stop here if the second address does not respond. Confirm the address was reprogrammed correctly by connecting only that module and running the address query sketch.

## Stage 3: verify CT clamp response

1. With the test sketch running, clamp the CT around a known load cord (e.g. desk lamp or fan)
2. Turn the load on
3. Confirm current and power readings increase to nonzero values
4. Turn the load off
5. Confirm current drops back to near zero

This step validates that the CT clamp is correctly oriented and making good contact.

## Stage 4: flash circuit monitor firmware

1. Flash the circuit monitor serial JSON sketch
2. Open the serial monitor at `115200`
3. Ignore the boot comment lines that start with `#`
4. Copy one full JSON packet line into a file named `packet.json`

Expected boot comments:

```text
# circuit_monitor_serial_json
# UART2 pin plan: RX=GPIO16 TX=GPIO17
# PZEM address 0x01: responding
# PZEM address 0x02: responding
```

Expected packet behavior:

- one JSON object per line
- one packet about every 10 seconds
- circuits array contains one object per responding PZEM module
- `inferred_state` reflects current load condition

## Stage 5: validate state classification

1. With no load on the monitored circuit, confirm `inferred_state` reads `off` (HVAC) or `standby` (sump)
2. Apply a load that falls within a configured threshold band
3. Confirm `inferred_state` changes to the expected active state
4. Confirm `cycle_active` changes to `true` and `cycle_duration_s` begins counting
5. Remove the load
6. Confirm `inferred_state` returns to `off` or `standby`
7. Confirm `cycle_active` returns to `false` and `cycle_duration_s` returns to `null`

## Stage 6: validate packet locally

From the project directory:

```bash
python3 -m json.tool packet.json
```

Confirm valid JSON with the expected fields matching `serial-json-contract.md`.

Once the `equipment.circuit.snapshot` observation family is implemented in the ingest path, the validation command will be:

```bash
python3 -m oesis.ingest.ingest_packet packet.json --parcel-id parcel_demo_001
```

## Pass criteria

- PZEM module(s) respond to Modbus queries at configured address(es)
- voltage reads within 5% of expected line voltage
- current reads near zero with no load, nonzero with load applied
- serial JSON sketch prints one valid packet line every 10 seconds
- `inferred_state` transitions correctly when load is applied and removed
- `cycle_active` and `cycle_duration_s` track transitions accurately
- `python3 -m json.tool packet.json` succeeds

## Known first-build limitations

- `observed_at` remains a placeholder if Wi-Fi time sync is not configured or fails
- the `equipment.circuit.snapshot` observation family is not yet implemented in the ingest path, so full ingest validation is not yet possible
- current-draw thresholds are factory defaults and may not match specific installed equipment
- sump pump `starting` state detection requires the faster 5-second sample cadence and may miss very brief transients

## If something fails

- no PZEM response: check UART wiring (`GPIO16`/`GPIO17`), confirm PZEM has AC power, verify Modbus address
- voltage reads zero: PZEM voltage terminals are not connected to AC mains
- current reads zero with load applied: CT clamp not fully closed, clamped around both wires instead of hot only, or CT clamp connector not seated in PZEM current terminals
- `inferred_state` stays `unknown`: current draw falls between configured threshold bands; adjust thresholds for the specific equipment
- Modbus CRC errors: reduce UART wire length, check for loose connections, confirm baud rate matches (9600 for PZEM default)
- second PZEM not responding: confirm address was changed to `0x02` before connecting to shared bus
