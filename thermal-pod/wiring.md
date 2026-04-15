# Wiring

## Controller

First-build controller assumptions:
- Raspberry Pi 5
- local OS and Python runtime on the device
- GPIO I2C bus enabled

## Sensors

First-build sensor assumptions:
- MLX90640 breakout on I2C
- fixed field of view under a hood or shroud
- no extra motion or visible-light camera hardware in the first version

## Power rails

Recommended first-build wiring:
- Pi `3V3` -> MLX90640 `VIN` or equivalent low-voltage input on the breakout
- Pi `GND` -> MLX90640 `GND`
- Pi `GPIO2` / SDA -> MLX90640 `SDA`
- Pi `GPIO3` / SCL -> MLX90640 `SCL`

## Pin map

Logical first-build map:
- `3V3` -> sensor power
- `GND` -> shared ground
- `GPIO2` -> I2C SDA
- `GPIO3` -> I2C SCL

## Notes on cable lengths and shielding

- Keep the thermal sensor wiring short in the first build.
- Avoid routing I2C next to noisy power lines or fans.
- Use a stable mount so cable strain does not change the field of view.

## Known risks

- Enclosure self-heating can bias the scene badly.
- A poor hood can create reflections or trap heat.
- A badly chosen field of view can create privacy problems even if only derived metrics are stored.
