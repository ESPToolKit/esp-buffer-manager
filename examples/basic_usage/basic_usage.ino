#include <ESPBufferManager.h>

void setup() {
    Serial.begin(115200);
    delay(1000);

    const bool usePSRAMBuffers = true;
    const size_t bytes = 4096;

    uint8_t *buffer = ESPBufferManager::allocateObjects<uint8_t>(bytes, usePSRAMBuffers);
    if (!buffer) {
        Serial.println("ESPBufferManager allocation failed");
        return;
    }

    for (size_t i = 0; i < bytes; ++i) {
        buffer[i] = static_cast<uint8_t>(i & 0xFF);
    }

    buffer = static_cast<uint8_t *>(ESPBufferManager::reallocate(buffer, bytes * 2, usePSRAMBuffers));
    if (!buffer) {
        Serial.println("ESPBufferManager reallocation failed");
        return;
    }

    Serial.print("PSRAM available: ");
    Serial.println(ESPBufferManager::isPSRAMAvailable() ? "yes" : "no");

    ESPBufferManager::deallocate(buffer);
    Serial.println("ESPBufferManager basic usage complete");
}

void loop() {
    delay(1000);
}
