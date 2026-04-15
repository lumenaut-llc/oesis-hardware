# Power Source Guide

## Purpose

Define the power architecture for v1.0 parcel kit nodes. Each node family has different power requirements based on deployment context (indoor bench, sheltered outdoor, outdoor exposed). This guide documents decided topology, power budgets, and open items.

## Indoor nodes (bench-air)

**Power source:** USB 5V from a standard wall adapter or powered USB hub.

- Always-on operation assumed; no battery backup required for bench deployment
- No special protection needed beyond a quality USB-C cable
- Power loss means data gap, not hardware damage
- Typical draw: ~150 mA steady, ~350 mA during Wi-Fi transmission peaks
- Any USB adapter rated 1A or above is sufficient

**Decided:** USB 5V bench power is the v1.0 indoor power path. No additional protection circuitry required for indoor deployment.

## Sheltered outdoor nodes (mast-lite)

**Power source:** USB 5V routed from an indoor outlet through a weatherproof cable path.

### Topology (decided)

- Power originates indoors from a standard USB wall adapter
- USB-C cable routes through a wall penetration, window pass-through, or drip-loop entry to the sheltered mounting position
- The node runs on 5V regulated power; no onboard voltage regulation beyond the ESP32-S3 DevKitC-1 built-in regulator

### Protection requirements

- **Fuse or polyfuse:** recommended on the 5V line before the cable enters the outdoor zone. A 500 mA polyfuse provides overcurrent protection without requiring replacement after a transient event.
- **Drip loop:** the cable must form a drip loop before entering the enclosure, so water follows the cable downward rather than into the gland.
- **Cable gland:** IP67-rated cable gland at the enclosure entry point. The gland must grip the cable jacket, not individual conductors.
- **Strain relief:** cable must be strain-relieved inside the enclosure so connector forces do not stress solder joints or the USB port.
- **UV protection:** outdoor cable runs should use UV-rated jacket material, or be routed inside conduit or under eave cover.

### Power budget

| Component | Typical (mA) | Peak (mA) |
|-----------|-------------|-----------|
| ESP32-S3 (active, no Wi-Fi) | 80 | 100 |
| ESP32-S3 (Wi-Fi TX) | 250 | 350 |
| SHT45 (measuring) | 1 | 2 |
| BME680 (gas heater active) | 12 | 15 |
| **Total** | **~150** | **~350** |

A 1A USB adapter provides comfortable margin for steady-state and peak draw.

### Power-loss behavior

- ESP32-S3 reboots cleanly on power restoration; no graceful shutdown required
- Observation gap during outage is expected and tracked by freshness scoring
- No local buffering survives power loss unless FRAM or flash-based ring buffer is added (deferred; see field-hardening checklist)

## Flood node

**Power source:** USB 5V, same topology as mast-lite but with additional waterproofing considerations.

- Flood nodes are mounted at low points where water accumulates; the power entry point must be above the expected water line or sealed to IP67 standard
- Cable routing must account for potential water contact along the entire outdoor run
- Consider a higher mounting position for the power entry even if the sensor faces downward
- Power budget is similar to mast-lite (~150 mA typical, ~350 mA peak)

**Battery backup:** not required for v1.0 Tier A. For Tier B field deployment, a small LiPo with charge controller could provide 4-8 hours of backup, but this is deferred pending field experience with power reliability.

## Weather-PM-mast

**Power source:** USB 5V with dedicated 5V fan power.

- PM sensor fans draw additional current (~100 mA per fan cycle)
- Total power budget approximately 250 mA typical, 500 mA peak
- A 2A USB adapter is recommended
- Same outdoor cable routing and protection requirements as mast-lite

## Decided vs open items

### Decided (v1.0 Tier A)

- All nodes use USB 5V as the primary power source
- Indoor nodes require no additional power protection
- Outdoor nodes require drip loop, cable gland, and strain relief
- Power loss results in data gap tracked by freshness scoring
- No battery backup required for Tier A internal reference

### Open (Tier B / future)

- Named vendor and SKU for recommended USB adapters
- Named vendor for IP67 cable glands and polyfuses
- Battery backup design for flood node
- Solar power feasibility for remote installations
- Power-over-Ethernet (PoE) as alternative for longer cable runs
- Supercapacitor-based graceful shutdown for flash-write protection

## Related

- `field-hardening-checklist.md` -- power verification steps
- `../mast-lite/build-guide.md` -- mast-lite assembly including power entry
- `integrated-parcel-kit-bom.md` -- procurement list
- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/current/v1.0-parcel-kit-architecture.md` -- kit architecture overview
