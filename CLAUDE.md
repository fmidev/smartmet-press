# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project overview

`smartmet-press` (binary name: `qdpress`) is a weather chart rendering tool that reads FMI QueryData and a product definition file (`.pre`), producing PostScript (EPS) or ASCII text output. Part of the SmartMet Server ecosystem by the Finnish Meteorological Institute.

## Build commands

```bash
make                # Build the qdpress binary
make clean          # Clean build artifacts
make format         # Run clang-format on all source
make rpm            # Build RPM package (clean + build)
make install        # Install qdpress to $(bindir)
```

There is no test suite — the `test/` directory does not exist. Testing is done manually by running `qdpress` against `.pre` definition files.

## Dependencies

- `smartmet-library-newbase` — QueryData format, command line parsing, string utilities
- `smartmet-library-macgyver` — general utilities
- Boost (iostreams)

Build config is loaded from `smartbuildcfg`:
```makefile
include $(shell echo $${PREFIX-/usr})/share/smartmet/devel/makefile.inc
```

## Architecture

Single-binary tool (`main/qdpress.cpp`) with a library of ~49 source files.

### Class hierarchy

```
NFmiDescription              — base: definition file parser (reads tokens, numbers, strings)
  NFmiPressDescription       — adds press environment (paths, fonts, colors, encoding)
    NFmiPressTimeDescription — adds time handling (relative hours, time loops)
      NFmiPressProduct       — top-level product: reads .pre files, manages data/segments, writes EPS
      NFmiPressManager       — orchestrates repeated product generation from .prem files (varying time/station)
      NFmiParamRect          — base for data-dependent PS elements (also inherits NFmiRect)
        NFmiSymbolParamRect  — weather symbols
        NFmiNumberParamRect  — numeric values
        NFmiTextParamRect    — text values
        NFmiLetterParamRect  — letter-based output
        NFmi2SymbolParamRect, NFmiRotatingParamRect, NFmiScalingParamRect,
        NFmiSubstituteParamRect, NFmiExtremePlaceParamRect, NFmiExtremeTimeParamRect,
        NFmiSunTimeParamRect, NFmiTimeParamRect
```

### Key processing flow

1. `qdpress` reads command-line options and a `.pre` definition file
2. `NFmiPressProduct::ReadDescriptionFile()` preprocesses the file (resolve `#Include`, conditional directives like `#ifConditionDate`, variable substitution `$#name`)
3. `NFmiPressManager::ReadDescriptionAndWrite()` checks for a matching `.prem` manager file — if found, it orchestrates repeated product writes with varying parameters (stations, times)
4. `NFmiPressProduct::WritePS()` generates the PostScript/EPS output

### Definition file format

- `.pre` files — product definitions, must start with `#!PressProduct`
- `.prem` files — manager definitions for batch production
- Objects are declared with `#` directives (e.g. `#Param`, `#SymbolGroup`, `#Text`)
- Supports conditional blocks (`#ifWinterTime`, `#ifConditionDate`, `#ifConditionDay`), variables (`$#name`), and `#Include`

Reference documentation is in `docs/Qdpress-Reference-Manual.md` and `docs/Syntax-for-qdpress-definition.md`.

### PostScript output

`NFmiPsWriting` and `NFmiPressScaling` handle PS code generation. Color support includes both RGB and CMYK (`NFmiPressTypes.h`). Page sizes: Letter, A4, A3 (portrait and landscape), Universal.

## Code conventions

- C++17, clang-format style: Google-based, Allman braces, 100-column limit
- Class naming: `NFmi` prefix throughout (inherited from FMI's legacy naming)
- Member variables: `its` prefix for value members, `f` prefix for boolean flags
- Many comments and log messages are in Finnish
- Headers in `include/`, implementations in `source/`, entry point in `main/`

## CI

CircleCI builds RPMs for RHEL8 and RHEL10 using `fmidev/smartmet-cibase-{8,10}` Docker images. No automated tests are run in CI (build-only).
