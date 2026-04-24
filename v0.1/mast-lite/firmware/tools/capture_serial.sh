#!/bin/sh
set -eu

if [ "$#" -lt 1 ] || [ "$#" -gt 2 ]; then
  echo "Usage: $0 <serial-device> [output-log]"
  exit 1
fi

device="$1"
output="${2:-serial.log}"

stty -f "$device" 115200 cs8 -cstopb -parenb -ixon -ixoff -crtscts
echo "Capturing serial output from $device to $output"
echo "Press Ctrl-C to stop."
cat "$device" | tee "$output"
