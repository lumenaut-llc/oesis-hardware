# Serial JSON Contract

## Purpose

Define the exact serial payload shape the weather-pm-mast should emit during first-build bring-up so PM-first outdoor firmware output can be checked locally before any persistent transport exists.

## Transport rule

- emit one complete JSON object per line
- use UTF-8 text over the serial monitor
- keep the serial monitor at `115200`
- do not mix debug chatter into the same line as JSON

## Required packet shape

```json
{
  "schema_version": "oesis.weather-pm-mast.v1",
  "node_id": "weather-pm-mast-01",
  "observed_at": "2026-03-30T19:45:00Z",
  "firmware_version": "0.1.0",
  "location_mode": "outdoor",
  "sensors": {
    "sht45": {
      "present": true,
      "temperature_c": 17.9,
      "relative_humidity_pct": 66.2
    },
    "bme680": {
      "present": true,
      "temperature_c": 18.6,
      "relative_humidity_pct": 64.4,
      "pressure_hpa": 1013.9,
      "gas_resistance_ohm": 149880
    },
    "sps30": {
      "present": false
    }
  },
  "derived": {
    "temperature_c_primary": 17.9,
    "relative_humidity_pct_primary": 66.2,
    "pressure_hpa": 1013.9,
    "voc_trend_source": "gas_resistance_ohm"
  },
  "health": {
    "uptime_s": 1820,
    "wifi_connected": false,
    "free_heap_bytes": 214332,
    "read_failures_total": 0,
    "last_error": null
  }
}
```

## Current firmware behavior

The first scaffold supports:

- serial-only bring-up with placeholder timestamps
- optional Wi-Fi/NTP time sync if credentials are configured
- an `sps30` block that stays `present: false` until real SPS30 transport integration is added

## Expected PM-first expansion

Once the SPS30 driver is integrated, the `sps30` block should expand to:

```json
"sps30": {
  "present": true,
  "pm1_ugm3": 3.2,
  "pm25_ugm3": 8.7,
  "pm4_ugm3": 10.1,
  "pm10_ugm3": 11.4,
  "typical_particle_size_um": 1.8
}
```

| Field | Unit | Description |
|---|---|---|
| `pm1_ugm3` | ug/m3 | Mass concentration PM1.0 |
| `pm25_ugm3` | ug/m3 | Mass concentration PM2.5 |
| `pm4_ugm3` | ug/m3 | Mass concentration PM4.0 |
| `pm10_ugm3` | ug/m3 | Mass concentration PM10 |
| `typical_particle_size_um` | um | Typical particle size |

The SPS30 communicates over I2C (address `0x69`) or UART. The I2C path is
preferred for this build since the I2C bus is already used for SHT45 and
BME680. The sensor requires 5V power and a 10-second measurement startup
before readings stabilize.

**Integration checklist for the SPS30 contributor:**

1. Add the Sensirion SPS30 library to `platformio.ini` lib_deps
2. Initialize the SPS30 in `setup()` after the I2C bus is configured
3. Call `sps30_start_measurement()` and wait for the first valid reading
4. Read mass concentration values in `loop()` alongside SHT45 and BME680
5. Populate the `sps30` block with `"present": true` and all five fields
6. Add `sps30_init_failed` to the error reporting path if the sensor is
   not detected at boot
7. Update `derived` to include a `pm25_ugm3` field for direct smoke evidence

This is the detail that makes this contract materially different from
`mast-lite`: a richer outdoor smoke-evidence surface rather than only sheltered
weather context.
