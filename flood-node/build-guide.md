# Build Guide

## Summary

The first flood-node build should optimize for one thing: a repeatable dry reference at a documented low point. Do not try to solve parcel-wide flood inference on the first hardware pass.

## Current maturity

Default posture: experimental field prototype below a general `deployment maturity v1.0` claim.

**Program posture:** this is a **geography-gated hazard module**, not part of
the default `v0.1` current-truth slice or the first `v0.2` two-node parcel kit.
Build it only where parcel runoff context justifies a dedicated low-point lane.
See `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/node-taxonomy.md`.

## What this build actually adds

This build is for parcels where runoff or pooling is operationally important.
Its role is to add **low-point evidence**, not to redefine the entire product as
a flood-only system.

When it is justified, this node adds:

- low-point depth evidence
- rise-rate evidence over time
- repeatable geometry for later parcel-specific flood interpretation

It still does **not** provide:

- parcel-wide flood truth from one reading
- route/chokepoint awareness by itself
- pump-state or action verification by itself

Those later capabilities require the broader `v1.5` bridge and still-later
route/community surfaces.

## Required now

- rigid mount
- documented dry reference
- field marker or staff gauge posture
- enclosure posture that avoids becoming the flooded low point itself
- local logging or buffering posture for event review

## Add later

- stronger surge and power protection
- tamper or tilt detection
- more formal replacement and recalibration workflow

## Field-ready boundary

Do not describe this node as stronger parcel evidence until geometry, zero reference, and service posture are documented for the parcel.

## Serviceability notes

- treat mount drift as a service issue, not a minor nuisance
- keep geometry notes with the parcel record

## Required tools

- soldering iron and solder
- breadboard and jumper wires for bench bring-up
- multimeter
- small hand tools for bracket and enclosure assembly
- measuring tape or ruler
- USB-C data cable
- laptop with PlatformIO or Arduino support

## Required materials

- ESP32-S3 DevKitC-1
- MB7389 ultrasonic range sensor
- mounting bracket or fixed standoff
- weather-resistant enclosure
- voltage-divider parts or equivalent level protection for the analog output into ESP32 ADC
- hookup wire and strain relief

## Assembly steps

1. Inspect the ESP32, MB7389, and enclosure parts before soldering or mounting.
2. Build the bench harness first with short wires and an accessible sensor face.
3. Confirm power and analog wiring before closing the enclosure.
4. Flash the ADC smoke-test sketch and verify the raw reading changes when a flat target moves closer or farther away.
5. Flash the serial JSON sketch and confirm the node emits stable packets.
6. Record a dry reference distance before any field runoff testing.
7. Only then mount the node at the documented runoff low point.

## Wiring notes

- The first build assumes the ESP32 reads the MB7389 analog output through a divider or equivalent protection suitable for 3.3V ADC input.
- Keep power, ground, and signal routing short during bench bring-up.
- Treat the analog path as sensitive to noise and poor grounds.
- Keep the sensor face clear of enclosure edges, fasteners, and splash guards that could reflect the beam badly.

## Firmware setup

- Start with `flood_node_adc_smoke_test`.
- Confirm the raw ADC value and inferred sensor voltage move in the expected direction.
- Move to `flood_node_serial_json` only after bench behavior is monotonic and stable.
- Keep calibration constants provisional until the dry install distance is measured in the real mounting geometry.

## First test procedure

1. Power the ESP32 over USB-C.
2. Open serial at `115200`.
3. Run the smoke-test sketch and move a flat target through a few known distances.
4. Confirm the raw signal changes smoothly without obvious dropouts.
5. Flash the JSON sketch.
6. Verify packets include a plausible `distance_cm`, `water_depth_cm`, and `rise_rate_cm_per_hr`.
7. Save a sample packet and run it through the local ingest utilities.

## Field-test notes

- Dry-reference measurement matters more than pretty bench numbers.
- Record mount height, sensor angle, and low-point context with the first install.
- Expect to revise filtering and calibration after the first real runoff event.
- Do not interpret early depth values as parcel-wide flood severity.

## Maintenance

- inspect for mud, debris, spider webs, and splash residue
- verify the mounting angle has not drifted
- confirm cable strain relief and enclosure seals remain intact
- recheck dry-reference distance after any mechanical change

## Links to related docs

- `wiring.md`
- `firmware-notes.md`
- `calibration.md`
- `serial-json-contract.md`
- `operator-runbook.md`
