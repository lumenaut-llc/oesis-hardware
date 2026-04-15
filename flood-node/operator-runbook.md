# Flood Node Operator Runbook

## Purpose

This runbook covers the first flood-node bring-up from bench wiring to a validated local packet.

## What this runbook proves

This runbook proves the low-point flood evidence lane only:

- the node can emit repeatable low-point distance / depth packets
- the local ingest path can accept that packet family
- the parcel can gain point-specific runoff evidence where it is actually needed

It does **not** prove:

- parcel-wide flood truth
- route or chokepoint status
- sump / pump state
- intervention effectiveness after flood actions

## Step 1: Bench wire the node

- power the ESP32 over USB-C
- wire the MB7389 signal into `GPIO4` through the planned divider or equivalent protection
- share ground between the sensor and ESP32
- keep the first harness short and easy to inspect

## Step 2: Flash the ADC smoke test

From `flood-node/firmware`:

```bash
$HOME/Library/Python/3.11/bin/pio run -e flood_node_adc_smoke_test -t upload
```

Open serial at `115200` and confirm:
- raw ADC value changes when a flat target moves
- inferred voltage changes smoothly
- there are no repeated invalid-read messages

## Step 3: Flash the JSON sketch

```bash
$HOME/Library/Python/3.11/bin/pio run -e flood_node_serial_json -t upload
```

Confirm that one JSON packet is printed every 5 seconds.

## Step 4: Capture serial output

Use the helper in `firmware/tools/capture_serial.sh` to save a `serial.log`.

## Step 5: Extract the newest packet

From `oesis-runtime` repo root:

```bash
python3 -m oesis.ingest.extract_latest_packet /path/to/serial.log --output packet.json
```

## Step 6: Validate the packet locally

```bash
python3 -m oesis.ingest.ingest_packet packet.json
```

You should see a normalized observation printed to stdout.

## Step 7: Record dry-reference install context

Before any real runoff interpretation:
- measure and record the dry reference distance
- note the exact low-point location
- record mount angle and target surface
- keep the first install photos and notes with the node record

## Pass criteria

- ADC signal moves predictably on the bench
- serial JSON emits every 5 seconds without freezes
- local ingest accepts the packet
- dry-reference distance is documented

## Stop conditions

- raw ADC reads are pinned or noisy
- sensor voltage exceeds the expected protected range
- packets stall or contain repeated invalid readings
- the install point is not clearly a runoff low point
