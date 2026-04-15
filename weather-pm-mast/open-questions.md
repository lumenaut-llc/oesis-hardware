# Open Questions

- Should the first promoted build stay PM-first, or should wind and rain be required from day one?

  > **Recommended direction:** PM-first. Wind and rain add mechanical complexity that should not block the PM evidence path. Add wind/rain only after PM + temperature + humidity are stable for 30 days.

- What mast geometry best protects the PM path without starving airflow?

  > **Recommended direction:** Vertical mast with PM sensor intake at least 1.5m above ground, oriented away from dominant wind for splash protection. Airflow path should be naturally ventilated, not forced.

- How should PM maintenance state be reflected in packet health?

  > **Recommended direction:** Add a pm_cleaning_due boolean based on runtime hours since last cleaning. Firmware tracks hours; threshold is configurable. Health degrades to 'maintenance_due' when exceeded.

- Should this node share the bench-air schema lineage longer, or move immediately to a weather-pm-specific schema?

  > **Recommended direction:** Move to a weather-pm-specific schema (oesis.weather-pm.v1) from day one. The PM and optional weather fields are structurally different from bench-air and should not inherit that lineage.
