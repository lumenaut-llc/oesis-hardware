# Thermal Pod Serial JSON Contract

## Purpose

This document defines the first privacy-safe JSON payload for the thermal pod.

The contract intentionally excludes raw thermal frames from the normal packet path.

## Packet shape

```json
{
  "schema_version": "oesis.thermal-pod.v1",
  "node_id": "thermal-pod-01",
  "observed_at": "2026-03-31T21:15:00Z",
  "firmware_version": "0.1.0",
  "location_mode": "outdoor",
  "install_role": "fixed_scene",
  "privacy_mode": "derived_only",
  "sensors": {
    "mlx90640": {
      "present": true,
      "frame_width": 32,
      "frame_height": 24
    }
  },
  "derived": {
    "scene_min_c": 18.4,
    "scene_mean_c": 22.1,
    "scene_max_c": 34.7,
    "scene_spread_c": 16.3,
    "hot_fraction": 0.08,
    "hot_threshold_c": 30.0
  },
  "health": {
    "uptime_s": 600,
    "sample_interval_s": 10,
    "read_failures_total": 0,
    "last_error": null
  }
}
```

## Field guidance

- `schema_version`: keep fixed at `oesis.thermal-pod.v1` for the first build
- `node_id`: use a stable device id such as `thermal-pod-01`
- `location_mode`: typically `outdoor` or `sheltered`, based on the actual placement
- `install_role`: use `fixed_scene`
- `privacy_mode`: keep `derived_only` in the first build

## What this contract is for

This contract exists for derived scene-level thermal context only.

It is appropriate for:

- scene spread and hot-fraction trends
- fixed-view thermal monitoring
- privacy-preserving experimentation with area-based sensing

It is **not** appropriate for:

- raw-frame collection in the normal path
- person-identifying thermal workflows
- default parcel-kit claims

## Derived fields

- `scene_min_c`: coolest pixel-equivalent value in the frame
- `scene_mean_c`: arithmetic mean of the frame
- `scene_max_c`: warmest pixel-equivalent value in the frame
- `scene_spread_c`: `scene_max_c - scene_min_c`
- `hot_fraction`: share of cells at or above the configured hot threshold
- `hot_threshold_c`: threshold used for the hot-fraction calculation

## Health fields

- `uptime_s`: seconds since process start
- `sample_interval_s`: configured sample cadence
- `read_failures_total`: count of failed frame reads
- `last_error`: `null` when healthy, otherwise a short reason string

## Serial behavior

- one JSON object per line
- optional startup comments may begin with `#`
- no raw frame arrays in the normal line output

## Example line

```json
{"schema_version":"oesis.thermal-pod.v1","node_id":"thermal-pod-01","observed_at":"2026-03-31T21:15:00Z","firmware_version":"0.1.0","location_mode":"outdoor","install_role":"fixed_scene","privacy_mode":"derived_only","sensors":{"mlx90640":{"present":true,"frame_width":32,"frame_height":24}},"derived":{"scene_min_c":18.4,"scene_mean_c":22.1,"scene_max_c":34.7,"scene_spread_c":16.3,"hot_fraction":0.08,"hot_threshold_c":30.0},"health":{"uptime_s":600,"sample_interval_s":10,"read_failures_total":0,"last_error":null}}
```
