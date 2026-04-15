# Hardware Notice

## Purpose

This notice explains how to read the hardware subtree during the v0.1
open-release period.

## Intended license direction

The current intended direction for hardware design files is:

- `CERN-OHL-S v2`

The full license text is in `LICENSE` in this directory.

Firmware source under `hardware/` (for example in `*/firmware/`) follows the program’s **software** license direction (**GNU AGPL v3 or later**); see `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/software/LICENSE` and `https://github.com/lumenaut-llc/oesis-program-specs/blob/main/LICENSES.md`. The **CERN-OHL-S-2.0** text in `LICENSE` here governs **hardware design** materials (documentation of builds, wiring, mechanical design intent, and related design artifacts) in this subtree unless a more specific file notice says otherwise.

## Release boundary

During the current release period:

- approved hardware artifacts may be public under their attached notices and licenses
- publication of a build guide or high-level description does not mean every hardware design file is released
- exact release scope should be checked against the program-level notices and non-release controls

## Safety and claims boundary

Hardware materials should be read together with calibration, maintenance, and claims-limitation guidance.

Nothing in this subtree should be read as a guarantee of safety certification, field suitability, or emergency-grade performance unless explicitly stated after review.

## Read these first

- `../NOTICE.md`
- `../legal/ip.md`
- `../legal/public-preview-scope.md`
- `../legal/privacy/claims-and-safety-language.md`
