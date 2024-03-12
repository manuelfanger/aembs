#include "dtnOLED.h"

#include "McuRTOS.h"

static TaskHandle_t notifyTaskHandle = NULL;

static void notifyTask(void *pv) {
    for(;;) {
      xTaskNotifyTake();
    }
}

void DtnOLED_Init(void) {
    BaseType_t res;

    res = xTaskCreate(notifyTask, "notifyTask", 600/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+2, &notifyTaskHandle);
    if (res!=pdPASS) {
      /* error! */
      for(;;) {}
    }

    xTaskToNotify = xTaskGetCurrentTaskHandle();

}

void DtnOLED_Deinit(void) {

}