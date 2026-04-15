# Thermal Pod Operator Runbook

## Purpose

This runbook covers the first thermal-pod bring-up from Pi wiring to a validated derived-metrics packet.

## What this runbook proves

This runbook proves only the privacy-preserving derived thermal lane:

- the pod can emit derived thermal metrics
- the workflow can validate a derived-only packet
- no raw frame needs to travel in the normal packet path

It does **not** make this node part of the default parcel kit, and it does
not by itself justify route, adaptation, or occupant-protection claims.

## Step 1: Prepare the Raspberry Pi

- boot the Pi
- enable I2C
- confirm Python 3 is available
- place the pod in a bench-safe fixed scene

## Step 2: Install dependencies

From `thermal-pod/firmware`:

```bash
python3 -m pip install -r requirements.txt
```

## Step 3: Run the frame probe

```bash
python3 thermal_pod_frame_probe.py --once
```

Confirm:
- the MLX90640 is detected
- a plausible frame min and max are reported
- no raw frame file is written

## Step 4: Run the JSON emitter

```bash
python3 thermal_pod_serial_json.py --once
```

Confirm one JSON line is printed with:
- `privacy_mode` set to `derived_only`
- scene min, mean, max, and hot fraction
- health fields populated

## Step 5: Save a local packet

```bash
python3 thermal_pod_serial_json.py --once > packet.json
```

## Step 6: Validate with local ingest tools

From `oesis-runtime` repo root:

```bash
python3 -m oesis.ingest.ingest_packet /path/to/packet.json
```

If the generic ingest path does not yet recognize this schema, keep the packet as a recorded example for the next contract pass.

## Step 7: Freeze the field of view

Before treating comparisons as meaningful:
- document the scene
- confirm no obvious privacy-sensitive view is included
- fix the mount and hood geometry
- record the hot-threshold setting

## Pass criteria

- frame reads are stable
- derived-only JSON emits cleanly
- no raw frame is persisted in the normal path
- field of view is documented and privacy-reviewed

## Stop conditions

- sensor init fails repeatedly
- enclosure heat or sun dominates the scene
- the field of view creates privacy concerns
- output drifts because the mount is not stable
