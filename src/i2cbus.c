#include "McuRTOS.h"

#include "i2cbus.h"

static SemaphoreHandle_t i2cMutex = NULL;

void I2CBus_Init(void) {
    i2cMutex = xSemaphoreCreateRecursiveMutex();
    if (i2cMutex == NULL);
    vQueueAddToRegistry(i2cMutex, "I2C Mutex");
}

void I2CBus_Lock(void) {
    if (xSemaphoreTakeRecursive(i2cMutex, portMAX_DELAY) != pdPASS) {
        /* ERROR */
    }
    return;
}

void I2CBus_Unlock(void) {
    if (xSemaphoreGiveRecursive(i2cMutex) != pdPASS) {
        /* ERROR */
    }
    return;
}

void I2CBus_Deinit(void) { }