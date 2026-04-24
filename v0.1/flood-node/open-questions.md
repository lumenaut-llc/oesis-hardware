# Open Questions

- Should the long-term preferred MB7389 interface remain the analog path, or should the project switch to a different output mode after first bring-up?

  > **Recommended direction:** Analog path for first bring-up. Evaluate serial (RS232/TTL) after analog is proven — serial gives richer diagnostics but adds wiring complexity.

- Which install metadata belongs in firmware constants versus parcel context records?

  > **Recommended direction:** Dry reference distance and sensor orientation in firmware constants (they're physical install facts). Drainage geometry and low-point rationale in parcel context records (they're site interpretation).

- What minimum field-validation evidence is required before flood-node depth can influence stronger parcel outputs?

  > **Recommended direction:** At least 3 storm events with manual ruler cross-checks before flood-node depth influences any non-unknown parcel output. Document each event in a validation log.

- How aggressive should filtering be before it starts hiding meaningful rapid-rise events?

  > **Recommended direction:** Light median filtering (3-sample window) to reject single-reading spikes. No smoothing that could hide a 5-minute rise-rate event. Publish raw and filtered in the same packet.

- What is the right enclosure and splash-guard geometry for the first outdoor deployment?

  > **Recommended direction:** Sensor opening faces downward at a fixed standoff (minimum 0.3m above expected max water level). Splash guard extends 2x sensor diameter around the opening. Document geometry with install photos.
