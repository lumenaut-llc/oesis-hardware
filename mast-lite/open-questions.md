# Open Questions

- What enclosure vent pattern gives enough airflow without inviting wind-driven rain?

  > **Recommended direction:** Louvered vent with downward-facing openings and internal drip shelf. Test with simulated wind-driven rain before field deployment.

- Should mast-lite stay strictly sheltered, or count as an outdoor node once shielded correctly?

  > **Recommended direction:** Classify as sheltered outdoor once radiation shield and vent posture are proven. The distinction matters for inference trust scoring, not just labeling.

- When should UV move from optional to standard in this node?

  > **Recommended direction:** Optional through v1.0. Move to standard only when the inference engine can use UV data to improve parcel-state confidence.

- How much packet differentiation should exist between bench-air-node and mast-lite versus keeping the same schema version initially?

  > **Recommended direction:** Stay on the same oesis.bench-air.v1 schema lineage with metadata flags (siting: outdoor, shielded: true). Diverge only when packet content requires new fields.

- What mounting distance from walls or rooflines should the project standardize on for first deployments?

  > **Recommended direction:** Minimum 0.5m from walls/rooflines for first deployments. Document as a siting rule in the installation checklist, not a firmware constant.

- What exact runtime and acceptance evidence should be required before `mast-lite` support counts toward a promoted **program-phase `v0.2`** slice rather than remaining partial?

  > **Recommended direction:** At minimum, 30 days of continuous outdoor operation with documented health, freshness, and packet integrity. Mast-lite acceptance adds to bench-air acceptance, does not replace it.
