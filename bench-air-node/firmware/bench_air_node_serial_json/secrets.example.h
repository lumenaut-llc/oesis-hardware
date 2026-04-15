#pragma once

// Copy this file to secrets.h and fill in your local Wi-Fi credentials
// if you want NTP-backed observed_at timestamps during bench bring-up.

#define BENCH_AIR_WIFI_SSID ""
#define BENCH_AIR_WIFI_PASSWORD ""

// Optional: HTTP POST each packet to the OESIS reference ingest API (same LAN).
// Full URL including path, e.g. "http://192.168.1.50:8787/v1/ingest/node-packets"
// Leave empty to disable network upload (USB serial only).
#define BENCH_AIR_INGEST_URL ""

// Sent as X-OESIS-Parcel-Id on each POST (must match your parcel context in software).
#define BENCH_AIR_PARCEL_ID "parcel_demo_001"
