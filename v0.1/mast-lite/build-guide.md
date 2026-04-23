# Build Guide

## Summary

Build a sheltered outdoor parcel node around an ESP32-S3 DevKitC-1 with an SHT45 in a radiation-shielded position and a BME680 in a vented enclosure zone. The first-build goal is a stable sheltered outdoor prototype that preserves the bench-air packet contract while adding enclosure and siting discipline.

## Current maturity

Default posture: early `deployment maturity v1.0` target, but not field-ready by default.

**Program posture:** this build guide supports the sheltered-outdoor half of the
**program-phase `v0.2`** next-promotion parcel kit. Building the node does not
by itself prove that the widened slice is promoted; field-hardening, runtime
behavior, and evidence still need to line up. See
`https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/version-and-promotion-matrix.md`.

This guide still covers only the first outdoor critical-path lane. The newer
taxonomy also names `indoor-response-node` and `power-outage-node` as priority
**`v1.5`** additions, but those bridge surfaces are outside the build boundary
of this file.

## What this build actually proves

This build proves the first honest outdoor evidence lane for the parcel kit:

- sheltered outdoor temperature, humidity, pressure, and trend context
- outdoor siting, enclosure, and serviceability discipline
- shared packet lineage between indoor and sheltered-outdoor nodes
- first two-node parcel evidence composition for the same parcel

It does **not** yet prove:

- direct PM-based smoke confirmation
- richer wind and rainfall mechanics
- indoor response or occupant protection outcomes
- outage continuity or backup-power posture

## Why this remains ahead of weather-pm-mast

`mast-lite` should stay ahead of `weather-pm-mast` in the first integrated kit
because the product still needs the simplest outdoor critical-path node before a
richer mast lane. This build is about establishing the parcel-edge reference
surface with the smallest viable outdoor hardware burden.

## How this build connects to the first closed loop

Under the newer staged product direction, this node is the outdoor half of the
first serious smoke-protection closed loop:

- outdoor conditions from `mast-lite` now
- later indoor PM / temperature / RH from `indoor-response-node`
- later equipment-state such as recirculation, fan, and purifier status
- later action / outcome verification over a bounded response window

That means this guide should be read as the outdoor evidence prerequisite for
the later **hazard → house state → action → verified outcome** chain, not as a
complete adaptation build by itself.

## Required now

- vented enclosure with moisture posture
- connectorized or otherwise stable wiring
- cable-gland and strain-relief posture
- sheltered placement discipline
- local buffering or storage plan if the node will be described as deployed

## Add later

- stronger watchdog and device-health posture
- improved radio and antenna posture for hard sites
- richer maintenance workflow

## Field-ready boundary

Do not describe a mast-lite install as field-ready unless the shared field-hardening checklist is satisfied for the active parcel.

## Serviceability notes

- preserve enclosure access after mounting
- document vent, gland, and cable-routing choices in the install record

## Required tools

- soldering iron with fine tip
- solder
- flush cutters and small pliers
- breadboard or temporary perfboard for bench bring-up
- multimeter
- USB-C cable for programming and power
- drill or enclosure punch tools if mounting the first outdoor enclosure

## Required materials

- 1 ESP32-S3 DevKitC-1 with 3.3 V logic
- 1 SHT45 breakout
- 1 BME680 breakout
- 1 radiation shield or similarly ventilated sensor guard for the SHT45
- 1 vented enclosure for controller and sheltered sensor wiring
- short jumper wires and headers
- mast, bracket, or eave-mount hardware for sheltered placement
- optional UV sensor only after the base node is stable

## Preflight

1. Reuse the bench bring-up pattern before sealing anything outdoors.
2. Confirm you have a known-good USB data cable.
3. Confirm headers and solder joints are solid before moving to enclosure work.
4. Plan cable routing so the SHT45 stays separated from board heat and trapped enclosure air.

## Assembly steps

1. Build and validate the sensor stack on the bench first using the same shared `3V3`, `GND`, `SDA`, and `SCL` bus pattern as the bench air node.
2. Keep the project pin map at `GPIO8` for `SDA` and `GPIO9` for `SCL`.
3. Wire the SHT45 first, confirm `0x44`, then add the BME680 and confirm `0x76` or `0x77`.
4. Run the serial JSON sketch indoors before moving into the enclosure.
5. Place the SHT45 where airflow is cleanest, ideally in a simple radiation shield or similarly shaded vented position.
6. Place the BME680 where it remains dry and ventilated without being packed tightly against the controller.
7. Route cables to avoid drip paths into the enclosure.
8. Mount the enclosure and sensor assembly in a sheltered outdoor location before long-run testing.

## Power entry

For sheltered outdoor deployment, power must be routed from an indoor source through a weatherproof cable path.

- **Source:** USB 5V from a standard wall adapter (1A minimum) located indoors.
- **Cable routing:** USB-C cable routes through a wall penetration or window pass-through to the sheltered mounting position. Form a drip loop before the cable enters the enclosure.
- **Gland:** use an IP67-rated cable gland at the enclosure entry. The gland must grip the cable jacket, not individual conductors.
- **Protection:** a 500 mA polyfuse on the 5V line is recommended before the cable exits the indoor zone.
- **Strain relief:** secure the cable inside the enclosure so connector forces do not stress the USB port or solder joints.
- **Power-loss behavior:** the ESP32-S3 reboots cleanly on power restoration. Observation gaps during outages are tracked by freshness scoring in the runtime.

For detailed power budgets and protection rationale, see `../parcel-kit/power-source-guide.md`.

## Wiring notes

- Both sensors share the same I2C bus.
- Use 3.3 V power only.
- Keep wiring compact during bench validation, then secure and strain-relieve wires before outdoor mounting.
- `SDI` on many BME680 breakouts maps to `SDA`, and `SCK` maps to `SCL`.
- Keep the SHT45 physically farther from enclosure heat and regulator heat than the BME680 if possible.

## Firmware setup

1. Assign a stable `node_id` such as `mast-lite-01`.
2. Start with the scanner sketch, then the serial JSON sketch.
3. Use the same `oesis.bench-air.v1` packet shape for first outdoor bring-up.
4. Keep `location_mode` at `sheltered` or `outdoor` depending on the chosen deployment.
5. Enable optional Wi-Fi time sync only after the base sensor stack is stable.

The exact first-build serial payload target is defined in `serial-json-contract.md`.
The first firmware scaffold lives in `firmware/mast_lite_serial_json/mast_lite_serial_json.ino`.

## First test procedure

1. Validate the node indoors on USB power before closing the enclosure.
2. Confirm the scanner sees `0x44` and `0x76` or `0x77`.
3. Confirm the serial JSON sketch prints valid packets at `115200`.
4. Move the node into a sheltered outdoor position and rerun the same packet checks.
5. Leave the node stationary for 30 to 60 minutes and watch for resets, vanishing devices, or implausible heat spikes.
6. Compare SHT45 temperature and humidity trends against the bench air node or another trusted nearby reference.

## Field-test notes

- Use sheltered placement first: under an eave, porch roof, or similar rain-protected location.
- Avoid direct morning or afternoon sun on the enclosure.
- Keep the SHT45 away from walls, metal poles, and surfaces that reradiate heat.
- Do not treat this first outdoor build as weatherproof until condensation and splash behavior are observed.

## Maintenance

- inspect vents and cable glands after weather exposure
- recheck sensor detection after enclosure changes
- inspect for moisture, corrosion, and UV-damaged cable insulation
- rerun a short side-by-side check after any relocation

## Links to related docs

- `README.md`
- `wiring.md`
- `firmware-notes.md`
- `serial-json-contract.md`
- `operator-runbook.md`
- `firmware/README.md`
- `calibration.md`
