# Serial JSON Contract

## Purpose

Define the exact serial payload shape the circuit monitor node should emit so firmware output can be checked locally before any Wi-Fi or HTTP transport exists.

## Transport rule

- emit one complete JSON object per line
- use UTF-8 text over the serial monitor
- keep the serial monitor at `115200`
- do not mix debug chatter into the same line as JSON

During first-build bring-up it is fine to print a few human-readable boot messages, but every packet line should be valid standalone JSON.

## Required packet shape

```json
{
  "schema_version": "oesis.circuit-monitor.v1",
  "node_id": "circuit-monitor-01",
  "firmware_version": "0.1.0",
  "uptime_s": 3600,
  "observed_at": "2026-04-13T14:30:00Z",
  "circuits": [
    {
      "circuit_id": "hvac_main",
      "current_a": 9.42,
      "power_w": 1084.0,
      "voltage_v": 121.3,
      "power_factor": 0.95,
      "energy_kwh": 2.47,
      "inferred_state": "compressor_running",
      "cycle_active": true,
      "cycle_duration_s": 482
    },
    {
      "circuit_id": "sump_primary",
      "current_a": 0.08,
      "power_w": 1.2,
      "voltage_v": 121.4,
      "power_factor": 0.12,
      "energy_kwh": 0.31,
      "inferred_state": "standby",
      "cycle_active": false,
      "cycle_duration_s": null
    }
  ],
  "health": {
    "wifi_rssi": -62,
    "heap_free": 198432,
    "sample_interval_ms": 10000,
    "read_failures_total": 0,
    "last_error": null
  }
}
```

## Field guidance

- `schema_version`: keep fixed at `oesis.circuit-monitor.v1` for the initial specification
- `node_id`: stable per device, such as `circuit-monitor-01`
- `firmware_version`: match the flashed firmware build
- `uptime_s`: monotonic seconds since last boot
- `observed_at`: use an RFC 3339 UTC timestamp when real time is available
- `circuits`: array of circuit objects, one per monitored channel (1 or 2)
- `circuit_id`: operator-assigned string identifying the monitored circuit, set in firmware config
- `current_a`: RMS current in amperes, read from PZEM module
- `power_w`: active power in watts, read from PZEM module
- `voltage_v`: line voltage in volts, read from PZEM module
- `power_factor`: dimensionless ratio (0.0 to 1.0), read from PZEM module
- `energy_kwh`: cumulative energy since PZEM counter reset
- `inferred_state`: firmware-classified equipment state based on current-draw thresholds
- `cycle_active`: `true` if equipment is currently in an active cycle (above idle threshold)
- `cycle_duration_s`: seconds since current cycle started, `null` if `cycle_active` is `false`
- `wifi_rssi`: Wi-Fi signal strength in dBm, `null` if not connected
- `heap_free`: free heap memory in bytes
- `sample_interval_ms`: current sample interval in milliseconds
- `wifi_connected`: implied by presence of `wifi_rssi`; if Wi-Fi is not connected, `wifi_rssi` is `null`

## Inferred state enumerations

### HVAC circuits

| State | Description |
| --- | --- |
| `off` | Current below idle threshold |
| `fan_only` | Current in fan-only range, below compressor threshold |
| `compressor_running` | Current in compressor range (cooling mode) |
| `heating_active` | Current in heating range (gas furnace blower or heat strips) |
| `overload` | Current above maximum expected threshold |
| `unknown` | Current does not match any configured threshold band |

### Sump pump circuits

| State | Description |
| --- | --- |
| `standby` | Current below idle threshold |
| `starting` | Brief current spike at pump startup (transient, typically 1-3 seconds) |
| `running` | Current in normal running range |
| `overload` | Current above maximum expected threshold |
| `unknown` | Current does not match any configured threshold band |

## Current-draw signature reference

These are typical current-draw ranges for classification. Actual thresholds should be configurable per circuit in firmware config.

### Central AC (120V control circuit + blower)

| State | Typical current range |
| --- | --- |
| Idle | < 0.3 A |
| Fan only | 2 -- 4 A |
| Compressor running | 8 -- 15 A |

### Central AC (240V compressor circuit)

| State | Typical current range |
| --- | --- |
| Idle | < 0.3 A |
| Fan only | 1 -- 2 A |
| Compressor running | 4 -- 8 A |

### Gas furnace (120V blower circuit)

| State | Typical current range |
| --- | --- |
| Idle | < 0.3 A |
| Blower running | 3 -- 6 A |

### Heat pump (240V)

| State | Typical current range |
| --- | --- |
| Idle | < 0.3 A |
| Fan only | 1 -- 2 A |
| Compressor running | 4 -- 10 A |

### Sump pump (120V)

| State | Typical current range |
| --- | --- |
| Standby | < 0.2 A |
| Starting (transient spike) | ~1.5 A |
| Running | 3 -- 8 A |
| Overload | > 8 A |

## What this contract can and cannot mean

This contract is the baseline equipment-state evidence contract for circuit-level monitoring.

It is appropriate for:

- HVAC operating mode evidence (off, fan, compressor, heating)
- sump pump operational state evidence
- equipment cycle timing and duty cycle
- power draw anomaly detection
- energy consumption tracking

It is **not** sufficient by itself for:

- determining whether indoor conditions improved after HVAC activation
- verifying that a sump pump successfully lowered water level
- diagnosing specific equipment faults beyond gross overload
- replacing a qualified HVAC technician's assessment

This contract provides the equipment-state layer. Pairing it with indoor environment observations (from `bench-air-node` or `indoor-response-node`) enables the house-response verification loop described in the v1.5 bridge.

## First-build fallback for time

If the node does not yet have a real clock, use one of these approaches consistently:

- emit boot-relative timestamps only in debug logs and wait to emit JSON packets until time is available
- emit a placeholder RFC 3339 timestamp and clearly document that it is provisional during serial-only bring-up

The preferred path is to emit valid RFC 3339 UTC timestamps once Wi-Fi/NTP or another clock source exists.

## Serial line example

This is the shape the serial monitor should show on a single line:

```json
{"schema_version":"oesis.circuit-monitor.v1","node_id":"circuit-monitor-01","firmware_version":"0.1.0","uptime_s":3600,"observed_at":"2026-04-13T14:30:00Z","circuits":[{"circuit_id":"hvac_main","current_a":9.42,"power_w":1084.0,"voltage_v":121.3,"power_factor":0.95,"energy_kwh":2.47,"inferred_state":"compressor_running","cycle_active":true,"cycle_duration_s":482},{"circuit_id":"sump_primary","current_a":0.08,"power_w":1.2,"voltage_v":121.4,"power_factor":0.12,"energy_kwh":0.31,"inferred_state":"standby","cycle_active":false,"cycle_duration_s":null}],"health":{"wifi_rssi":-62,"heap_free":198432,"sample_interval_ms":10000,"read_failures_total":0,"last_error":null}}
```

## Local check path

1. Copy one emitted JSON line into a file such as `packet.json`
2. Validate the JSON structure with `python3 -m json.tool packet.json`
3. Once the `equipment.circuit.snapshot` observation family is implemented in the ingest path, run the appropriate normalization command from the `oesis-runtime` repo root

If JSON validation succeeds and the fields match this contract, the packet shape is good enough for the local bring-up target.
