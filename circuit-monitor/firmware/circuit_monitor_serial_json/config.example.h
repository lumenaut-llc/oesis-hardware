// Circuit monitor configuration.
// Copy this file to config.h and adjust for your installation.
// Do not commit config.h.

#ifndef CIRCUIT_MONITOR_CONFIG_H
#define CIRCUIT_MONITOR_CONFIG_H

// --- Node identity ---
#define CM_NODE_ID "circuit-monitor-01"
#define CM_FIRMWARE_VERSION "0.1.0"

// --- UART pins ---
#define CM_PZEM_RX_PIN 16
#define CM_PZEM_TX_PIN 17

// --- Circuit 0: HVAC ---
#define CM_CIRCUIT_0_ENABLED true
#define CM_CIRCUIT_0_ID "hvac_main"
#define CM_CIRCUIT_0_TYPE "central_ac_120v"
#define CM_CIRCUIT_0_ADDRESS 0x01
#define CM_CIRCUIT_0_SAMPLE_INTERVAL_MS 10000
#define CM_CIRCUIT_0_IDLE_MAX_A 0.3
#define CM_CIRCUIT_0_FAN_MIN_A 2.0
#define CM_CIRCUIT_0_FAN_MAX_A 4.0
#define CM_CIRCUIT_0_COMPRESSOR_MIN_A 8.0
#define CM_CIRCUIT_0_COMPRESSOR_MAX_A 15.0
#define CM_CIRCUIT_0_OVERLOAD_A 15.0

// --- Circuit 1: Sump pump ---
#define CM_CIRCUIT_1_ENABLED true
#define CM_CIRCUIT_1_ID "sump_primary"
#define CM_CIRCUIT_1_TYPE "sump_pump_120v"
#define CM_CIRCUIT_1_ADDRESS 0x02
#define CM_CIRCUIT_1_SAMPLE_INTERVAL_MS 5000
#define CM_CIRCUIT_1_IDLE_MAX_A 0.2
#define CM_CIRCUIT_1_RUNNING_MIN_A 3.0
#define CM_CIRCUIT_1_RUNNING_MAX_A 8.0
#define CM_CIRCUIT_1_OVERLOAD_A 8.0

// --- Wi-Fi (optional, for NTP time sync) ---
// Leave empty to skip Wi-Fi and use boot-relative timestamps.
#define CM_WIFI_SSID ""
#define CM_WIFI_PASSWORD ""

#endif // CIRCUIT_MONITOR_CONFIG_H
