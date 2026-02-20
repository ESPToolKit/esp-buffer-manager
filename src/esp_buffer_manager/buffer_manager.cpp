#include "buffer_manager.h"

#include <cstdlib>

#if __has_include(<esp_heap_caps.h>)
extern "C" {
#include <esp_heap_caps.h>
}
#define ESP_BUFFER_MANAGER_HAS_HEAP_CAPS 1
#else
#define ESP_BUFFER_MANAGER_HAS_HEAP_CAPS 0
#endif

#if defined(ARDUINO) && __has_include(<esp32-hal-psram.h>)
#include <esp32-hal-psram.h>
#define ESP_BUFFER_MANAGER_HAS_ARDUINO_PSRAM_FOUND 1
#else
#define ESP_BUFFER_MANAGER_HAS_ARDUINO_PSRAM_FOUND 0
#endif

namespace {
#if ESP_BUFFER_MANAGER_HAS_HEAP_CAPS
constexpr uint32_t kAnyCaps = MALLOC_CAP_8BIT;
#if defined(MALLOC_CAP_INTERNAL)
constexpr uint32_t kInternalCaps = MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT;
#else
constexpr uint32_t kInternalCaps = MALLOC_CAP_8BIT;
#endif
#if defined(MALLOC_CAP_SPIRAM)
constexpr uint32_t kPsramCaps = MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT;
#endif

void *allocateAny(size_t bytes) {
    return heap_caps_malloc(bytes, kAnyCaps);
}

void *allocateInternal(size_t bytes) {
    void *mem = heap_caps_malloc(bytes, kInternalCaps);
    if (!mem && kInternalCaps != kAnyCaps) {
        mem = allocateAny(bytes);
    }
    return mem;
}

void *allocatePsram(size_t bytes) {
#if defined(MALLOC_CAP_SPIRAM)
    void *mem = heap_caps_malloc(bytes, kPsramCaps);
    if (mem) {
        return mem;
    }
#endif
    return allocateInternal(bytes);
}

void *reallocateAny(void *ptr, size_t bytes) {
    return heap_caps_realloc(ptr, bytes, kAnyCaps);
}

void *reallocateInternal(void *ptr, size_t bytes) {
    void *mem = heap_caps_realloc(ptr, bytes, kInternalCaps);
    if (!mem && kInternalCaps != kAnyCaps) {
        mem = reallocateAny(ptr, bytes);
    }
    return mem;
}

void *reallocatePsram(void *ptr, size_t bytes) {
#if defined(MALLOC_CAP_SPIRAM)
    void *mem = heap_caps_realloc(ptr, bytes, kPsramCaps);
    if (mem) {
        return mem;
    }
#endif
    return reallocateInternal(ptr, bytes);
}
#endif
}  // namespace

void *ESPBufferManager::allocate(size_t bytes, bool usePSRAMBuffers) noexcept {
    if (bytes == 0) {
        return nullptr;
    }

#if ESP_BUFFER_MANAGER_HAS_HEAP_CAPS
    if (shouldUsePSRAM(usePSRAMBuffers)) {
        return allocatePsram(bytes);
    }
    return allocateInternal(bytes);
#else
    (void)usePSRAMBuffers;
    return std::malloc(bytes);
#endif
}

void *ESPBufferManager::reallocate(void *ptr, size_t bytes, bool usePSRAMBuffers) noexcept {
    if (!ptr) {
        return allocate(bytes, usePSRAMBuffers);
    }
    if (bytes == 0) {
        deallocate(ptr);
        return nullptr;
    }

#if ESP_BUFFER_MANAGER_HAS_HEAP_CAPS
    if (shouldUsePSRAM(usePSRAMBuffers)) {
        return reallocatePsram(ptr, bytes);
    }
    return reallocateInternal(ptr, bytes);
#else
    (void)usePSRAMBuffers;
    return std::realloc(ptr, bytes);
#endif
}

void ESPBufferManager::deallocate(void *ptr) noexcept {
    if (!ptr) {
        return;
    }

#if ESP_BUFFER_MANAGER_HAS_HEAP_CAPS
    heap_caps_free(ptr);
#else
    std::free(ptr);
#endif
}

bool ESPBufferManager::isPSRAMAvailable() noexcept {
#if ESP_BUFFER_MANAGER_HAS_ARDUINO_PSRAM_FOUND
    return psramFound();
#elif ESP_BUFFER_MANAGER_HAS_HEAP_CAPS && defined(MALLOC_CAP_SPIRAM)
    return heap_caps_get_total_size(MALLOC_CAP_SPIRAM) > 0;
#else
    return false;
#endif
}

bool ESPBufferManager::shouldUsePSRAM(bool usePSRAMBuffers) noexcept {
    return usePSRAMBuffers && isPSRAMAvailable();
}
