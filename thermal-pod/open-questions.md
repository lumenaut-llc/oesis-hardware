# Open Questions

- What fixed scene geometry is acceptable from both a privacy and usefulness standpoint?

  > **Recommended direction:** Fixed downward-facing view (45-60 degree angle) covering a defined ground area no larger than 5m x 5m. Privacy constraint: no window, door, or walkway in the field of view.

- Should the first build support configurable masked regions, or is whole-scene derived output enough?

  > **Recommended direction:** Whole-scene derived output is sufficient for the first build. Masking adds complexity that should wait until the first build proves which scene features matter.

- Which derived metrics are actually worth keeping beyond min, mean, max, spread, and hot fraction?

  > **Recommended direction:** Keep min, mean, max, spread, and hot fraction. Add temporal derivative (rate of change over 5-minute window) as the sixth metric. Other metrics should be justified by inference utility before adding.

- How should enclosure self-heating be measured and bounded in the field?

  > **Recommended direction:** Measure internal enclosure temperature alongside scene temperature. If delta exceeds 5C, flag in health as 'enclosure_thermal_bias'. Validate with a shaded reference thermometer during install.

- Is there any acceptable future case for retaining a raw frame snapshot, or should the node remain derived-only permanently?

  > **Recommended direction:** Derived-only permanently in the default operating mode. Any future frame capture must be a separate, explicitly consented research mode with its own retention policy and governance gate.
