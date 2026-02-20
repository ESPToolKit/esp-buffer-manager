# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

### Added
- Initial ESPBufferManager implementation with PSRAM-aware `allocate`, `reallocate`, and `deallocate` helpers.
- Added `isPSRAMAvailable()` and `shouldUsePSRAM()` runtime helpers.
- Added Arduino example (`examples/basic_usage`) demonstrating safe fallback behavior.

## [0.1.0] - 2026-02-20

### Added
- First release scaffold for ESPBufferManager.
- Shared, non-throwing memory management API for ESPToolKit libraries.
- Arduino/ESP-IDF compatible platform guards.
- CI, release workflow, issue templates, and PR template.
