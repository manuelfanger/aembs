
#include "csem.h"
#include "sensor.h"
#include "leds.h"

#include "McuRTOS.h"
#include "McuUtility.h"
#include "McuSystemView.h"

static SemaphoreHandle_t countingSemaphore = NULL;
static TaskHandle_t BossTaskHandle = NULL;
static TaskHandle_t WorkerTaskHandle = NULL;

void BossTask(void *pv) {
    BaseType_t res;

    xSemaphoreGive(countingSemaphore);
    xSemaphoreGive(countingSemaphore);
    xSemaphoreGive(countingSemaphore);

    if (countingSemaphore != NULL) {
        for(;;) {
            if (xSemaphoreTake(countingSemaphore,portMAX_DELAY) == pdTRUE) {
                res = xTaskCreate(WorkerTask, "Worker Task", 1000/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+1, &WorkerTaskHandle);
                if (res!=pdPASS) {
                    for(;;) {} /* error! */
                }
            }
            else {
                /*Unreachable, because xBlockTime is set to portMAX_DELAY*/
            }
        }
    }
}

void WorkerTask(void *pv) {
    int randNum;
    randNum = McuUtility_random(10,50);
    for(;;) {
        vTaskDelay(pdMS_TO_TICKS(randNum));
        xSemaphoreGive(countingSemaphore);
        vTaskDelete(NULL);
    }
}

void Csem_Init(void) {
    int seed;
    float temperature, humidity;
    temperature = GetTemp(); /* get values for random seed */
    humidity = GetHum(); /* get values for random seed */
    seed = (int)(temperature*10.0f) + (int)(humidity*10.0f);
    McuUtility_randomSetSeed(seed);

    countingSemaphore = xSemaphoreCreateCounting(3,3);
    if(countingSemaphore == NULL) {
        for(;;) { }
    }
    vQueueAddToRegistry(countingSemaphore, "Counting Semaphore");

    BaseType_t res;
    res = xTaskCreate(BossTask, "BossTask", 1000/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+2, &BossTaskHandle);
    if (res!=pdPASS) {
        /* error! */
        for (;;) { }
    }
}

void Csem_Deinit(void) {

}