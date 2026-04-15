# Wiring

## Summary

Wire one or two PZEM energy monitoring modules to an ESP32-S3 over a shared
UART bus. The ESP32 handles low-voltage Modbus RTU communication only. All
mains-voltage connections are on the PZEM side and must be treated with
appropriate safety precautions.

## Pin map

| ESP32-S3 pin | Function | Connects to |
|---|---|---|
| `GPIO16` | UART2 RX | PZEM TX (all modules, shared bus) |
| `GPIO17` | UART2 TX | PZEM RX (all modules, shared bus) |
| `USB-C` | Power + serial monitor | Host computer |

The ESP32 `Serial0` (USB) is used for JSON packet output and debug.
`Serial2` (`GPIO16`/`GPIO17`) is dedicated to PZEM Modbus RTU.

## PZEM module connections

Each PZEM-004T (120V) or PZEM-016 (240V) has two connection groups:

### Low-voltage side (UART)

| PZEM pin | Connects to |
|---|---|
| TX | ESP32 `GPIO16` (shared across all modules) |
| RX | ESP32 `GPIO17` (shared across all modules) |
| 5V | Not connected to ESP32 (PZEM powers itself from AC mains) |
| GND | ESP32 GND (common ground required for UART) |

### High-voltage side (mains)

| PZEM terminal | Connects to |
|---|---|
| Voltage L | AC hot (line) wire |
| Voltage N | AC neutral wire |
| CT input | SCT-013-030 split-core CT clamp plug |

## CT clamp placement

- Clamp around the **hot (line) wire only** — never neutral alone, never both
  wires together (fields cancel, reading will be near zero).
- For appliances on a standard 120V outlet, use a cord splitter or extension
  cord that separates hot and neutral to allow clamping the hot wire
  individually. This avoids any panel access.
- Ensure the CT clamp jaw is fully closed and seated around the wire.
- CT polarity affects power factor sign but not current magnitude. If power
  factor reads negative, reverse the clamp orientation.

## Multi-module bus wiring

When using two PZEM modules on one ESP32:

```
ESP32 GPIO16 (RX) ──┬── PZEM #1 TX
                     └── PZEM #2 TX

ESP32 GPIO17 (TX) ──┬── PZEM #1 RX
                     └── PZEM #2 RX

ESP32 GND ──────────┬── PZEM #1 GND
                     └── PZEM #2 GND
```

Modules are differentiated by Modbus address:
- PZEM #1: address `0x01` (factory default)
- PZEM #2: address `0x02` (must be reprogrammed before connecting to the
  shared bus — use the address-change sketch with only that module connected)

## Voltage level note

PZEM TX/RX signals are 5V TTL. Most ESP32-S3 GPIO pins tolerate 5V on input,
but check your specific board datasheet. For reliable long-term operation,
consider a bidirectional logic level shifter (3.3V to 5V) on the UART lines.

## Physical separation

Keep the ESP32 and all low-voltage wiring physically separated from
mains-voltage PZEM wiring. Do not route mains wires near the ESP32 board.
Use separate cable runs or a physical barrier inside any shared enclosure.

## Wire length

Keep UART wiring between the ESP32 and PZEM modules under 30 cm to avoid
Modbus communication errors. If longer runs are needed, consider shielded
cable or RS-485 conversion.
