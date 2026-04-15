# Firmware Source Stubs

PlatformIO `src/` entry points for circuit-monitor firmware variants.

- `circuit_monitor_serial_json.cpp` — Main serial JSON output firmware
- `circuit_monitor_pzem_test.cpp` — PZEM-004T/016 sensor test harness
- `circuit_monitor_address_change.cpp` — Modbus address configuration utility

The canonical Arduino sketch is in the parent `circuit_monitor_serial_json/`
directory. These `.cpp` stubs are PlatformIO wrappers.
