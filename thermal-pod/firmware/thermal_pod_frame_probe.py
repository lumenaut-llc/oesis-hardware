#!/usr/bin/env python3
import argparse
import json
import pathlib
import statistics
import sys

try:
    import board  # type: ignore
    import busio  # type: ignore
    import adafruit_mlx90640  # type: ignore
except ImportError:
    board = None
    busio = None
    adafruit_mlx90640 = None


FRAME_SIZE = 32 * 24


def load_config(config_path: pathlib.Path) -> dict:
    if not config_path.exists():
        return {}
    return json.loads(config_path.read_text())


def read_frame() -> list[float]:
    if adafruit_mlx90640 is None or board is None or busio is None:
        return [20.0 + ((index % 17) * 0.35) for index in range(FRAME_SIZE)]
    i2c = busio.I2C(board.SCL, board.SDA, frequency=400000)
    mlx = adafruit_mlx90640.MLX90640(i2c)
    mlx.refresh_rate = adafruit_mlx90640.RefreshRate.REFRESH_2_HZ
    frame = [0.0] * FRAME_SIZE
    mlx.getFrame(frame)
    return frame


def summarize_frame(frame: list[float]) -> dict:
    scene_min = min(frame)
    scene_max = max(frame)
    scene_mean = statistics.fmean(frame)
    return {
        "scene_min_c": round(scene_min, 2),
        "scene_mean_c": round(scene_mean, 2),
        "scene_max_c": round(scene_max, 2),
        "scene_spread_c": round(scene_max - scene_min, 2),
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--config", default="config.json")
    parser.add_argument("--once", action="store_true")
    args = parser.parse_args()

    config = load_config(pathlib.Path(args.config))
    _ = config

    try:
        frame = read_frame()
    except Exception as exc:  # pragma: no cover
        print(f"frame_read_failed: {exc}", file=sys.stderr)
        return 1

    summary = summarize_frame(frame)
    print(json.dumps(summary))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
