# Integrated Parcel Builder Checklist

## Purpose

Provide one practical implementation checklist for turning the current
architecture into a real first parcel build.

This file is narrower than the broader phase and product roadmaps.
It focuses on what a builder or implementation owner must put in place for the
current parcel-first delivery lane.

## Status

Current builder checklist spanning **program-phase `v0.1`** (Tier 1) and the **next promotion** integrated kit (**`v0.2`**, Tier 2). For how “current truth,” “next promotion,” and capability stages differ, read `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/version-and-promotion-matrix.md` and `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/node-taxonomy.md`.

## Use this checklist for

- Milestone 1: one parcel, one node
- Milestone 2: first integrated parcel kit

Use the milestone roadmap when deciding sequence and scope.
Use this file when deciding what must actually exist across hardware, firmware,
ingest, inference, parcel UX, and governance.

## Hardware

### Tier choice

- choose `Tier 1` for the fastest useful parcel slice
- choose `Tier 2` when the indoor reference path is already stable and you are
  ready to add sheltered outdoor evidence

### Minimum hardware for Tier 1

- one `bench-air-node`
- one stable indoor power path
- one chosen install role and room or area description
- one stable `node_id`

### Additional hardware for Tier 2

- one `mast-lite`
- one sheltered outdoor mount plan
- one stable outdoor power path
- one chosen install role and siting note
- one stable `node_id`

### Build checks

- standardize controller family and sensor stack for the first parcel kit
- validate each node on the bench before any permanent mounting
- confirm each node has a documented install role, `location_mode`, and
  calibration state
- keep the parcel kit modular rather than forcing all sensing into one chassis

## Firmware

### Packet and identity requirements

- keep a stable versioned packet contract per node family
- include `schema_version`, `node_id`, `observed_at`, `firmware_version`,
  `location_mode`, sensor payloads, and health fields
- preserve the common packet envelope across node families even when family
  details differ

### Bring-up requirements

- start with scanner or lowest-complexity validation firmware
- move next to serial JSON or equivalent inspectable packet output
- keep serial capture available for local troubleshooting even after live
  transport is added
- choose a stable sample cadence appropriate for bench validation and install
  checks

### Firmware acceptance checks

- device identity stays stable across reflashes unless intentionally changed
- packet cadence is repeatable
- sensor-presence and health fields are visible
- timestamps and freshness behavior are understandable

## Ingest

### Trust-boundary requirements

- accept packets at one clear ingest boundary
- validate structure, lineage, freshness, and basic identity
- bind packet meaning to parcel and node metadata rather than hiding that logic
  inside ad hoc scripts
- preserve raw evidence, receipt context, and quarantine behavior for bad
  packets

### Minimum ingest outputs

- canonical normalized observation
- ingest acknowledgement or error
- provenance and freshness context

### Ingest acceptance checks

- a real node packet can be submitted repeatedly without contract drift
- invalid packets are rejected or quarantined clearly
- normalization preserves the fields inference needs without pretending ingest
  is the hazard engine

## Inference

### Minimum reasoning requirements

- combine normalized local evidence with parcel context
- combine public context only through explicit, allowed lanes
- emit parcel condition outputs rather than hidden internal scores
- attach confidence, freshness, evidence mode, and reasons

### Scope rules

- keep hazard reasoning in inference, not in ingest
- do not let parcel presentation silently recompute inference
- add new observation families only when the contract and explanation surface
  are ready

### Inference acceptance checks

- parcel-state outputs remain understandable when evidence is sparse
- low-confidence and `unknown` states remain honest
- explanations reflect what evidence was actually available

## Parcel UX

### Minimum parcel-facing surface

- one current parcel answer
- one explanation path
- visible confidence and freshness framing
- visible evidence-mode framing
- source-aware evidence summary

### User-experience rules

- present the parcel as one system, not a pile of devices
- explain why the system believes the current answer
- avoid emergency-authority or guaranteed-safety language
- keep privacy and sharing controls part of the product surface

### Parcel UX acceptance checks

- a parcel operator can tell what is happening now
- a parcel operator can tell why the answer was produced
- a parcel operator can tell how current and how confident the answer is

## Governance and data model

### Minimum records

- one `parcel_id`
- one parcel-scoped node registry
- one parcel-context record
- one sample validated packet per installed node
- one install note per installed node

### Boundary rules

- exact parcel-linked private evidence stays private by default
- shared outputs stay downstream, thresholded, and policy-gated
- install metadata and calibration notes remain available for truthful
  interpretation and support

### Governance acceptance checks

- parcel-private and shared outputs are technically distinguishable
- node records include install role, transport mode, power mode, and
  calibration state
- retention, access, and export expectations are not contradicted by the
  implementation path

## Recommended implementation order

1. build and validate `bench-air-node`
2. validate packet output and ingest normalization
3. confirm parcel context and parcel inference are usable
4. confirm parcel view explains confidence, freshness, and reasons
5. harden parcel-scoped registry and install metadata
6. add and validate `mast-lite`
7. expand only after the singular parcel path remains clear

## What not to require too early

- dense neighborhood participation
- `weather-pm-mast` as first outdoor critical-path hardware
- `flood-node` on parcels where runoff is not operationally meaningful
- `thermal-pod` in the first integrated pilot
- a public parcel-resolution map

## References

- `README.md`
- `parcel-kit-procurement-checklist.md`
- `parcel-installation-checklist.md`
- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/system/integrated-parcel-system-spec.md`
- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/current/milestone-roadmap.md`
- `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/architecture/current/minimum-functioning-v0.1.md`
