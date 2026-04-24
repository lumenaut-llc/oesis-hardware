#!/usr/bin/env python3
import argparse
import json
import pathlib
import statistics
import sys
import time
from datetime import datetime, timezone

try:
    import board  # type: ignore
    import busio  # type: ignore
    import adafruit_mlx90640  # type: ignore
except ImportError:
    board = None
    busio = None
    adafruit_mlx90640 = None


FRAME_WIDTH = 32
FRAME_HEIGHT = 24
FRAME_SIZE = FRAME_WIDTH * FRAME_HEIGHT
START_TIME = time.time()


def load_config(config_path: pathlib.Path) -> dict:
    if not config_path.exists():
        return {
            "node_id": "thermal-pod-01",
            "location_mode": "outdoor",
            "install_role": "fixed_scene",
            "sample_interval_s": 10,
            "hot_threshold_c": 30.0,
            "privacy_mode": "derived_only",
        }
    return json.loads(config_path.read_text())


def read_frame() -> list[float]:
    if adafruit_mlx90640 is None or board is None or busio is None:
        return [18.0 + ((index % 23) * 0.55) for index in range(FRAME_SIZE)]
    i2c = busio.I2C(board.SCL, board.SDA, frequency=400000)
    mlx = adafruit_mlx90640.MLX90640(i2c)
    mlx.refresh_rate = adafruit_mlx90640.RefreshRate.REFRESH_2_HZ
    frame = [0.0] * FRAME_SIZE
    mlx.getFrame(frame)
    return frame


def build_packet(config: dict, frame: list[float], read_failures_total: int, last_error: str | None) -> dict:
    scene_min = min(frame)
    scene_max = max(frame)
    scene_mean = statistics.fmean(frame)
    hot_threshold = float(config["hot_threshold_c"])
    hot_fraction = sum(1 for value in frame if value >= hot_threshold) / len(frame)
    return {
        "schema_version": "oesis.thermal-pod.v1",
        "node_id": config["node_id"],
        "observed_at": datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
        "firmware_version": "0.1.0",
        "location_mode": config["location_mode"],
        "install_role": config["install_role"],
        "privacy_mode": config["privacy_mode"],
        "sensors": {
            "mlx90640": {
                "present": True,
                "frame_width": FRAME_WIDTH,
                "frame_height": FRAME_HEIGHT,
            }
        },
        "derived": {
            "scene_min_c": round(scene_min, 2),
            "scene_mean_c": round(scene_mean, 2),
            "scene_max_c": round(scene_max, 2),
            "scene_spread_c": round(scene_max - scene_min, 2),
            "hot_fraction": round(hot_fraction, 4),
            "hot_threshold_c": hot_threshold,
        },
        "health": {
            "uptime_s": int(time.time() - START_TIME),
            "sample_interval_s": int(config["sample_interval_s"]),
            "read_failures_total": read_failures_total,
            "last_error": last_error,
        },
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--config", default="config.json")
    parser.add_argument("--once", action="store_true")
    args = parser.parse_args()

    config = load_config(pathlib.Path(args.config))
    read_failures_total = 0

    while True:
        last_error = None
        try:
            frame = read_frame()
        except Exception as exc:  # pragma: no cover
            frame = [0.0] * FRAME_SIZE
            read_failures_total += 1
            last_error = f"frame_read_failed:{exc}"

        packet = build_packet(config, frame, read_failures_total, last_error)
        print(json.dumps(packet))
        sys.stdout.flush()

        if args.once:
            return 0
        time.sleep(float(config["sample_interval_s"]))


if __name__ == "__main__":
    raise SystemExit(main())
