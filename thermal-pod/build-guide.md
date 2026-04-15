# Build Guide

## Summary

The first thermal-pod build should optimize for a stable fixed scene and derived metrics only. Do not treat the first version as a general-purpose thermal camera product.

## Current maturity

Default posture: separate R&D lane below a general `deployment maturity v1.0` claim.

**Program posture:** this guide remains a **research- and privacy-gated** lane.
It should stay outside the default first integrated pilot until privacy,
retention, and operational usefulness are clearer. See
`https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/node-taxonomy.md`.

## Required now

- stable Pi 5 power posture
- high-endurance storage posture
- repeatable hood or aperture geometry
- fixed-view install notes

## Add later

- clean shutdown or backup power posture
- stronger timing posture
- richer masking and scene-stability tooling

## Field-ready boundary

Do not describe the thermal pod as field-ready unless the repo documents stable power, durable storage, clean shutdown posture, thermal isolation, and repeatable field-of-view geometry.

## Serviceability notes

- keep the hood, aperture, and storage posture inspectable
- treat mount drift as a service event

## Required tools

- screwdriver set
- soldering tools if headers or harnesses need assembly
- Raspberry Pi setup tools and power supply
- USB keyboard/monitor or SSH access for Pi setup
- multimeter
- hood or shroud fabrication tools as needed

## Required materials

- Raspberry Pi 5
- MLX90640 breakout
- hooded enclosure or thermal shroud
- short I2C harness
- stable mount with fixed view
- microSD storage and Pi power supply

## Assembly steps

1. Prepare the Raspberry Pi and confirm it boots cleanly.
2. Wire the MLX90640 on a short I2C run before placing it in the enclosure.
3. Verify the sensor is detected and a thermal frame can be read.
4. Run the frame-probe script first.
5. Run the derived-metrics JSON script and confirm one JSON line per interval.
6. Add the hooded enclosure only after the open-bench read path works.
7. Freeze the field of view before collecting any comparison logs.

## Wiring notes

- Keep the MLX90640 on a short, clean I2C connection.
- Share the Pi 3.3V and ground rails with the breakout as required by the selected board.
- Avoid long unshielded I2C runs in the first build.
- Do not aim the first install at windows, public sidewalks, or other obviously privacy-sensitive views.

## Firmware setup

- install the Python dependencies listed in `firmware/requirements.txt`
- start with `thermal_pod_frame_probe.py`
- move to `thermal_pod_serial_json.py` once frame reads succeed
- keep configuration in the local config file rather than hardcoding site-specific thresholds into code

## First test procedure

1. Power the Raspberry Pi and connect to the pod workspace.
2. Run the frame probe and confirm a plausible min/max spread.
3. Run the serial JSON script and watch one packet per cycle.
4. Save a log and feed one line through the local ingest utilities if desired.
5. Confirm no raw thermal frame is written to disk in the normal path.

## Field-test notes

- The hood geometry matters as much as the code.
- Fix the field of view before comparing logs across days.
- Record what surfaces are in scene so later trends are interpretable.
- Treat scene metrics as contextual evidence, not as parcel-wide truth.

## Maintenance

- inspect hood cleanliness and lens opening
- verify the mount has not shifted
- check for enclosure heat buildup
- confirm the scene has not materially changed

## Links to related docs

- `wiring.md`
- `firmware-notes.md`
- `calibration.md`
- `serial-json-contract.md`
- `operator-runbook.md`
