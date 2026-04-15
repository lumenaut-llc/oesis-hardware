# Calibration

## Summary

The circuit monitor node classifies equipment state by comparing measured RMS
current against configurable threshold bands. Calibration means setting those
thresholds to match the specific equipment connected to each monitored circuit.

## What calibration proves

- current-draw thresholds correctly classify the equipment's real operating
  modes (off, fan, compressor, running, etc.)
- cycle detection transitions match actual equipment start/stop events
- no persistent `unknown` states during normal operation

## When to calibrate

- at initial installation (always)
- when monitored equipment is replaced or serviced
- if `inferred_state` reports `unknown` during normal operation
- if cycle detection misses transitions or reports false cycles

## Procedure

### Step 1: Identify baseline idle current

1. Ensure the monitored equipment is fully off (not standby).
2. Run the PZEM test sketch or serial JSON sketch.
3. Record the current reading. This is the noise floor.
4. Set `idle_max_a` to 1.5x this value (typical: 0.2-0.3 A).

### Step 2: Identify active-mode current bands

For each distinct operating mode of the equipment:

1. Force the equipment into that mode (e.g., fan only, compressor, pump
   running).
2. Let it stabilize for 30-60 seconds past any startup transient.
3. Record the steady-state current range (min and max over 60 seconds).
4. Set the threshold band to cover this range with ~10% margin on each side.

### Step 3: Verify transitions

1. Cycle the equipment through all modes.
2. Confirm `inferred_state` changes correctly at each transition.
3. Confirm `cycle_active` toggles at equipment start and stop.
4. Adjust threshold bands if any transitions are missed or misclassified.

## Default threshold reference

These are starting-point thresholds. Adjust to match your specific equipment.

### HVAC — central AC (120V blower circuit)

| Parameter | Default |
|---|---|
| `idle_max_a` | 0.3 |
| `fan_min_a` | 2.0 |
| `fan_max_a` | 4.0 |
| `compressor_min_a` | 8.0 |
| `compressor_max_a` | 15.0 |
| `overload_a` | 15.0 |

### HVAC — central AC (240V compressor circuit)

| Parameter | Default |
|---|---|
| `idle_max_a` | 0.3 |
| `fan_min_a` | 1.0 |
| `fan_max_a` | 2.0 |
| `compressor_min_a` | 4.0 |
| `compressor_max_a` | 8.0 |
| `overload_a` | 8.0 |

### Sump pump (120V)

| Parameter | Default |
|---|---|
| `idle_max_a` | 0.2 |
| `running_min_a` | 3.0 |
| `running_max_a` | 8.0 |
| `overload_a` | 8.0 |

## PZEM address assignment

Each PZEM module on the shared UART bus needs a unique Modbus address.

1. Connect **only** the module to be reprogrammed to the ESP32 UART bus.
2. Flash and run the address-change sketch.
3. Set the target address (e.g., `0x02` for the second module).
4. Confirm the module responds at the new address.
5. Power off, then connect it to the shared bus alongside the first module.
6. Verify both addresses respond in the PZEM test sketch.

Factory default address is `0x01`. Only change the second (and subsequent)
modules.

## Ongoing validation

After initial calibration, spot-check thresholds periodically:

- If the equipment is serviced or replaced, recalibrate from step 1.
- If `inferred_state` reports `unknown` frequently, the current draw has
  shifted outside the configured bands — re-measure and adjust.
- If `cycle_active` triggers during idle, `idle_max_a` is set too low.
- If cycles are missed, the active-mode threshold is too high.
