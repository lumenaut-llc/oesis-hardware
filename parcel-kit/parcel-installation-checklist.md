# Parcel Installation Checklist

## Purpose

Turn the first parcel kit from a bench build into a repeatable field installation with enough documentation to support software binding, later troubleshooting, and truthful claims.

## Implementation status

- **Tier 1** matches the narrow **program-phase `v0.1`** reference path.
- **Tier 2** supports the **`v0.2`** promotion (indoor + sheltered outdoor). Completing Tier 2 install steps does not by itself prove runtime normalization or promotion — track **implementation status** separately (`https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/version-and-promotion-matrix.md`).

## Scope

This checklist covers:

- Tier 1 indoor reference installation with `bench-air-node`
- Tier 2 sheltered outdoor installation with `mast-lite`
- optional low-point documentation for `flood-node`

This checklist does not treat `weather-pm-mast` or `thermal-pod` as part of the default first parcel installation.

## Core install rule

One parcel can have multiple nodes, but the install must still behave like one system:

- one `parcel_id`
- one parcel-scoped node registry
- one install note set
- one software validation path

Do not mount hardware before the parcel identity, node IDs, and intended install roles are clear.

## Deployment maturity rule

Do not describe a node as deployed or field-ready unless the install also documents:

- protected power posture
- local buffering or storage posture
- serviceable wiring and connector posture
- enclosure support posture
- physical node identity label
- service access posture
- spare or replacement posture for the active family

## Before leaving the bench

Confirm these are already true:

- each node has passed its bench bring-up path
- each node has a stable `node_id`
- packet output is visible over serial JSON
- local ingest validation has succeeded at least once per node
- the node-registry draft includes the intended `node_id`, `hardware_family`, `location_mode`, and `install_role`

Primary references:

- `parcel-kit-procurement-checklist.md`
- `../data-model/node-registry-schema.md`
- `../bench-air-node/operator-runbook.md`
- `../mast-lite/operator-runbook.md`
- `../flood-node/operator-runbook.md`

## Parcel walk and install planning

Before mounting anything:

1. Confirm the parcel target and `parcel_id`.
2. Decide which tier is being installed:
   `Tier 1` for indoor reference only
   `Tier 2` for indoor plus sheltered outdoor reference
3. Pick a stable power path for each node.
4. Note obvious heat, moisture, sun, splash, and service-access risks.
5. Record where each node is supposed to sit and why that role matters to the parcel system.

## Tier 1: Bench-air-node indoor reference checklist

Use `bench-air-node` as the indoor reference node for the parcel.

### Siting rules

- prefer a lived-in but not overly disturbed indoor zone
- avoid direct sun through windows
- avoid immediate proximity to HVAC supply vents, return vents, or space heaters
- avoid kitchens, bathrooms, and garages unless those are the explicit test environments
- keep the node off the floor and away from enclosed cabinets
- keep the node accessible for USB power, serial debugging, and relocation

### Install metadata to record

- `node_id`
- `parcel_id`
- physical label or QR label state
- `location_mode: indoor`
- `install_role: indoor_reference`
- room or area description
- approximate mounting or shelf height
- nearby bias sources such as windows, vents, appliances, or doors
- power path posture
- local buffering or storage posture
- service access notes

### First on-site checks

1. Power the node in its chosen location.
2. Let it stabilize for at least 30 minutes.
3. Confirm packets continue without resets or missing sensors.
4. Compare the SHT45 trend against a nearby trusted indoor reference if available.
5. Mark `calibration_state` as `provisional` or `verified` based on what was actually checked.

### Stop conditions

- direct sun reaches the node during the expected operating window
- the node sits in forced-air flow or next to a strong heat source
- the location is too hidden to service or validate

## Tier 2: Mast-lite sheltered outdoor checklist

Use `mast-lite` as the first sheltered outdoor reference node for the same parcel.

### Siting rules

- start under an eave, porch roof, or similar rain-protected location
- keep the SHT45 in the cleanest ventilated position, ideally in a radiation shield
- keep the controller and BME680 dry inside or near the vented enclosure
- avoid direct sun on the enclosure where possible
- avoid wall faces, metal poles, grills, dryer vents, and vehicle exhaust zones
- keep cable runs short, strain-relieved, and protected from drip paths

### Install metadata to record

- `node_id`
- `parcel_id`
- physical label or QR label state
- `location_mode: sheltered`
- `install_role: parcel_edge_reference`
- mount type such as eave, porch, or bracket
- SHT45 shield position and airflow notes
- enclosure position relative to walls and reflected-heat surfaces
- cable-gland and vent posture
- local buffering or storage posture
- power protection posture

### First on-site checks

1. Bench-validate the node before closing the enclosure.
2. Mount the node and let it stabilize for at least 30 minutes.
3. Confirm packets continue without vanishing devices or implausible heat spikes.
4. Compare trends against the indoor node and any nearby trusted outdoor reference if available.
5. Keep `calibration_state` at `provisional` until the sheltered placement has survived a real temperature swing or weather change.

### Stop conditions

- direct rain or splash reaches the controller enclosure
- direct sun or wall radiation obviously biases the readings
- cable routing creates drip paths into the enclosure
- the location is hard to inspect after weather exposure

## Optional flood-node low-point documentation

Do not add `flood-node` by default.
Use this section only if the parcel truly has an operationally meaningful runoff low point.

### Site-selection rules

- the point must be a documented runoff low point, not just a convenient mount
- the sensor view should target a repeatable surface with stable geometry
- the mount must preserve a known angle and dry reference distance
- the node should remain serviceable after debris, splash, or storm exposure

### Documentation to capture before enabling stronger interpretation

- `node_id`
- `parcel_id`
- physical label or QR label state
- `location_mode: outdoor`
- `install_role: runoff_low_point`
- low-point description
- dry reference distance
- mount height
- mount angle
- target surface description
- install photos and notes kept with the node record
- field marker or staff gauge posture
- enclosure placement relative to splash and standing water

### First on-site checks

1. Confirm the low point still makes sense after physically standing at the site.
2. Record the dry reference distance immediately after mounting.
3. Save a sample packet and validate it locally.
4. Keep `enabled` or stronger interpretation conservative until the geometry and early field behavior are understood.

## Software and registry handoff

After installation, update the parcel-scoped node registry with:

- `installed_at`
- `calibration_state`
- `transport_mode`
- `power_mode`
- `enabled`
- `last_seen_at` after the first live packet arrives
- service and label notes where the active maturity target requires them

Also keep:

- one sample validated packet per installed node
- one short install note per node
- one parcel-level record of which node classes are active

Primary references:

- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/current/README.md`
- `../data-model/node-registry-schema.md`
- `../data-model/examples/node-registry.example.json`
- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/integrated-parcel-system-spec.md`
- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/software/operator-quickstart.md`

## Acceptance criteria for the first integrated parcel kit

Treat the install as complete only when:

- `bench-air-node` runs stably in an indoor reference location
- `mast-lite` runs stably in a sheltered outdoor location
- the node registry binds both nodes to one parcel
- each installed node has at least one validated packet path
- install notes explain the chosen siting and known bias risks
- the shared field-hardening bundle is documented for any node being described as deployed

## Legal and public-preview boundary

Installation notes are implementation material, not default public-preview material.

Before sharing install photos, wiring layouts, or close-up mounting details outside the core implementation group, check:

- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/legal/public-preview-scope.md`
- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/release/v0.1/reviewer-packet-index.md` (release label `v0.1`, filesystem path `v0.1/`)

The public preview site should not imply that every documented install practice is already a polished consumer deployment path.
