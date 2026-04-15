# Wiring

## Controller

First-build controller assumptions:
- ESP32-S3 DevKitC-1
- USB-C for bench power and programming
- single analog input for the distance signal

## Sensors

First-build sensor assumptions:
- MB7389 powered from the bench node supply used for that sensor variant
- analog output routed through a divider or equivalent protection into the ESP32 ADC pin
- common ground shared between sensor and MCU

## Power rails

Recommended bench bring-up rails:
- sensor supply rail appropriate for the installed MB7389 variant
- ESP32 `GND` shared with sensor `GND`
- protected analog signal into the ESP32 ADC input

## Pin map

Logical first-build map:

- `GND` -> shared ground
- `GPIO4` -> protected analog distance input
- `USB-C` -> board power and programming

Optional later additions:
- status LED or heartbeat output
- digital trigger/status lines if the final interface mode changes

## Bench layout

- Keep the analog run short and away from noisy USB bundles where possible.
- Put the sensor where a flat target can move cleanly in front of it.
- Avoid enclosure lips or breadboard parts in the measurement cone.

## Notes on cable lengths and shielding

- Longer sensor leads can add noise and make grounding problems look like distance jitter.
- If the final install needs longer cable runs, validate them after the short-wire bench build passes first.
- Use strain relief and weather-rated routing for any outdoor cable.

## Known risks

- Feeding an unprotected sensor analog output directly into the ESP32 ADC can damage the board or invalidate readings.
- Poor grounds or splash contamination can create false depth changes.
- A bad mounting angle can dominate the error budget even when the wiring is perfect.
