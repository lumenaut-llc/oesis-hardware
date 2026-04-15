# Firmware Notes

## Responsibilities

- boot reliably and initialize PZEM module communication over UART (Modbus RTU)
- sample current, voltage, power, power factor, and energy on a predictable cadence
- classify equipment state from current-draw thresholds
- track cycle transitions and compute cycle timing
- emit timestamped JSON packets with node and circuit metadata
- surface health telemetry and read failures without hiding them

This firmware should stay evidence-oriented. It should not attempt to encode parcel-state decisions, intervention advice, or verification logic in the node itself.

## Platform

- ESP32-S3 development board
- PZEM-004T (120V) or PZEM-016 (240V) energy monitoring module
- Communication: UART / Modbus RTU between ESP32 and PZEM
- SCT-013-030 split-core CT clamp (30A range) per monitored circuit

## PZEM address management

Each PZEM module on the same UART bus must have a unique Modbus address:

- first circuit (e.g. HVAC): address `0x01` (factory default)
- second circuit (e.g. sump pump): address `0x02` (must be reprogrammed before connecting to the shared bus)

To change a PZEM address, connect only that module to the bus and use the address-change command before adding the second module. The firmware should verify both addresses respond at boot and report missing modules in health telemetry.

## UART wiring

- PZEM TX to ESP32 `GPIO16` (UART RX)
- PZEM RX to ESP32 `GPIO17` (UART TX)
- both PZEM modules share the same UART bus (Modbus addresses differentiate them)
- use `Serial2` on ESP32-S3 for PZEM communication, keeping `Serial0` free for debug and JSON output

## Sampling cadence

- HVAC circuit: every 10 seconds (sufficient for mode detection and cycle timing)
- sump pump circuit: every 5 seconds (faster cadence to catch short pump cycles that may last only 15-30 seconds)
- warm-up period: discard or flag the first 2 to 3 samples after boot while PZEM stabilizes
- if only one circuit is configured, use the faster cadence for that circuit

The packet emission interval should match the slower cadence (10 seconds) and include the most recent reading from each circuit.

## Current-draw classification

The firmware classifies equipment state by comparing the measured RMS current against configurable threshold bands. Default thresholds are defined per circuit type in the firmware configuration.

Classification logic:

1. Read `current_a` from PZEM module
2. Compare against the configured threshold bands for the circuit type
3. Assign the matching `inferred_state` value
4. If current falls between defined bands or does not match any band, assign `unknown`

Thresholds should be stored in firmware configuration (not hardcoded) so they can be adjusted per install without reflashing. A typical configuration block:

```
circuit_0:
  id: "hvac_main"
  type: "central_ac_120v"
  address: 0x01
  sample_interval_ms: 10000
  thresholds:
    idle_max_a: 0.3
    fan_min_a: 2.0
    fan_max_a: 4.0
    compressor_min_a: 8.0
    compressor_max_a: 15.0
    overload_a: 15.0

circuit_1:
  id: "sump_primary"
  type: "sump_pump_120v"
  address: 0x02
  sample_interval_ms: 5000
  thresholds:
    idle_max_a: 0.2
    running_min_a: 3.0
    running_max_a: 8.0
    overload_a: 8.0
```

## Cycle detection

The firmware tracks equipment cycle transitions:

1. When current transitions from below `idle_max_a` to above the active threshold, record `cycle_start_time`
2. Set `cycle_active` to `true`
3. On each sample while active, compute `cycle_duration_s` as elapsed time since `cycle_start_time`
4. When current transitions back below `idle_max_a`, increment `cycle_count`, set `cycle_active` to `false`, set `cycle_duration_s` to `null`

For sump pump starting detection:

1. A brief current spike above running threshold followed by settling into the running range is classified as `starting` for the duration of the transient (typically 1-3 seconds)
2. After the transient settles, reclassify as `running`

## Packet shape

See `serial-json-contract.md` for the full packet definition.

## First-build serial target

During bring-up, the node should emit one complete JSON packet per serial line at `115200`. The exact line-oriented payload target is documented in `serial-json-contract.md`.

Practical first-build rules:

- do not mix JSON and human-readable debug text on the same line
- set `wifi_rssi` to `null` during serial-only bring-up
- include only circuits that have a responding PZEM module at boot
- if a PZEM module stops responding, continue publishing remaining circuits with explicit error reporting

## ESPHome compatibility

This node can alternatively be built using the ESPHome PZEM-AC platform component, which provides native Modbus RTU support for PZEM-004T and PZEM-016 modules. The ESPHome path trades custom firmware control for faster bring-up and native Home Assistant integration. If the ESPHome path is used, the serial JSON contract still defines the target packet shape for any custom output component.

## Error handling

- if a PZEM module fails to respond, continue publishing data from remaining modules with explicit presence and error fields
- count consecutive read failures per PZEM address
- trigger a soft restart only after a defined failure threshold, not on a single bad read
- never silently substitute stale values for fresh reads
- expose boot reason and last error in debug logs
- report Modbus communication errors (timeout, CRC failure, unexpected response) in health telemetry

## Health telemetry

- uptime
- free heap memory
- Wi-Fi signal strength if transport is enabled
- PZEM module presence per address at boot
- cumulative read failures per circuit
- sample interval per circuit
- packet sequence counter

## Future enhancements

- local ring buffer for offline replay
- NTP-backed clock sync with explicit drift reporting
- multi-PZEM expansion beyond two circuits
- BLE or captive-portal provisioning
- signed packets if node identity assurance becomes important
- harmonic analysis for more precise equipment identification
