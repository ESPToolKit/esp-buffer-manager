#pragma once

#include <cstddef>
#include <cstdint>

class ESPBufferManager {
   public:
    static void *allocate(size_t bytes, bool usePSRAMBuffers = false) noexcept;
    static void *reallocate(void *ptr, size_t bytes, bool usePSRAMBuffers = false) noexcept;
    static void deallocate(void *ptr) noexcept;

    template <typename T>
    static T *allocateObjects(size_t count, bool usePSRAMBuffers = false) noexcept {
        if (count == 0) {
            return nullptr;
        }
        if (count > (static_cast<size_t>(-1) / sizeof(T))) {
            return nullptr;
        }
        return static_cast<T *>(allocate(count * sizeof(T), usePSRAMBuffers));
    }

    static bool isPSRAMAvailable() noexcept;
    static bool shouldUsePSRAM(bool usePSRAMBuffers) noexcept;
};
