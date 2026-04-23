#!/usr/bin/env sh
set -eu

if [ "$#" -ne 2 ]; then
  echo "usage: $0 <serial-device> <output-log>"
  exit 1
fi

SERIAL_DEVICE="$1"
OUTPUT_LOG="$2"

stty -f "$SERIAL_DEVICE" 115200 raw -echo -icanon
cat "$SERIAL_DEVICE" | tee "$OUTPUT_LOG"
