# ESPBufferManager

ESPBufferManager is a small shared utility for ESPToolKit libraries that centralizes PSRAM-aware memory allocation. It keeps behavior non-throwing and always falls back to standard/internal heap paths when PSRAM is not available.

## CI / Release / License
[![CI](https://github.com/ESPToolKit/esp-buffer-manager/actions/workflows/ci.yml/badge.svg)](https://github.com/ESPToolKit/esp-buffer-manager/actions/workflows/ci.yml)
[![Release](https://img.shields.io/github/v/release/ESPToolKit/esp-buffer-manager?sort=semver)](https://github.com/ESPToolKit/esp-buffer-manager/releases)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE.md)

## Features
- Single API for buffer allocation decisions across ESPToolKit libraries.
- PSRAM-preferred allocation when `usePSRAMBuffers` is enabled.
- Automatic fallback to internal/default heap when PSRAM is unavailable.
- Non-throwing API surface (`nullptr` on allocation failure).
- Works with Arduino-ESP32 and ESP-IDF builds.

## Examples
Quick start:

```cpp
#include <ESPBufferManager.h>

bool usePSRAMBuffers = true;
size_t bytes = 2048;

uint8_t* data = ESPBufferManager::allocateObjects<uint8_t>(bytes, usePSRAMBuffers);
if (!data) {
    // handle OOM
    return;
}

data = static_cast<uint8_t*>(ESPBufferManager::reallocate(data, bytes * 2, usePSRAMBuffers));
if (!data) {
    // handle OOM
    return;
}

ESPBufferManager::deallocate(data);
```

Runnable sketch:
- `examples/basic_usage` – allocates, fills, reallocates, and frees a buffer using PSRAM-preferred mode.

## Gotchas
- `usePSRAMBuffers = true` is best-effort; it does not guarantee all allocations land in PSRAM.
- When PSRAM is not present (or allocation fails), allocation falls back to non-PSRAM heap paths.
- This library only manages allocations done through `ESPBufferManager`; it does not control third-party/internal STL allocations automatically.

## API Reference
- `void* ESPBufferManager::allocate(size_t bytes, bool usePSRAMBuffers = false)`
- `void* ESPBufferManager::reallocate(void* ptr, size_t bytes, bool usePSRAMBuffers = false)`
- `void ESPBufferManager::deallocate(void* ptr)`
- `template <typename T> T* ESPBufferManager::allocateObjects(size_t count, bool usePSRAMBuffers = false)`
- `bool ESPBufferManager::isPSRAMAvailable()`
- `bool ESPBufferManager::shouldUsePSRAM(bool usePSRAMBuffers)`

## Restrictions
- Designed for ESP32-class targets (`architectures=esp32`).
- Requires C++17 (`-std=gnu++17`).
- Does not throw exceptions; callers must check returned pointers.

## Tests
A dedicated host test suite is not included yet. Use the `examples/basic_usage` sketch and CI build matrix to verify integration.

## License
ESPBufferManager is released under the [MIT License](LICENSE.md).

## ESPToolKit
- Check out other libraries: <https://github.com/orgs/ESPToolKit/repositories>
- Hang out on Discord: <https://discord.gg/WG8sSqAy>
- Support the project: <https://ko-fi.com/esptoolkit>
- Visit the website: <https://www.esptoolkit.hu/>
