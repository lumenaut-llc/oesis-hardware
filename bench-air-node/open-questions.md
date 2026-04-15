# Open Questions

- Should the MVP publish over serial only, or require Wi-Fi delivery to the ingest service before the subsystem counts as complete?

  > **Recommended direction:** Serial-first for v0.1 reference. Wi-Fi required for v1.0 live deployment but serial remains the bring-up and debug path.

- What enclosure standard should be used for sheltered deployments so airflow remains useful without inviting dust or splashes?

  > **Recommended direction:** Define a minimum airflow aperture spec and splash exclusion zone rather than mandating a specific enclosure model. Document the spec in the build guide.

- Should the node compute any local anomaly hints, or stay strictly evidence-only and leave all interpretation to downstream services?

  > **Recommended direction:** Evidence-only. All interpretation downstream. Nodes should not compute anomaly scores — this preserves clean evidence boundaries.

- Which sensor should be canonical for temperature and humidity when SHT45 and BME680 disagree?

  > **Recommended direction:** SHT45 canonical for temperature and humidity (higher accuracy spec). BME680 provides gas resistance and pressure as complementary channels, not competing truth.

- Do we want a persistent local ring buffer for offline periods in the first hardware iteration, or is continuous power plus upstream ingest enough?

  > **Recommended direction:** Defer to v1.0. Continuous power plus upstream ingest is sufficient for v0.1 reference. Local ring buffer becomes important once Wi-Fi transport is standard.

- What node identity scheme should be used before any secure provisioning workflow exists?

  > **Recommended direction:** Operator-assigned string ID (e.g., 'bench-air-01') burned into firmware config for v0.1. Stronger provisioning (per-node token or certificate) deferred to v1.5+.
