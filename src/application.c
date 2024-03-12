#include "McuRTOS.h"
#include "McuSystemView.h"
#include "McuDebounce.h"

#include "application.h"
#include "leds.h"
#include "myTasks.h"
#include "buttons_config.h"

#include <stdint.h>
#include <stdio.h>

#define EVENT_QUEUE_LENGTH    (5)
#define EVENT_QUEUE_ELEM_SIZE (sizeof(ApplicationEvent_t))

static TaskHandle_t queueTaskHandle = NULL;
static QueueHandle_t eventQueueHandle;

typedef struct {
  BTN_Buttons_e button;
  McuDbnc_EventKinds kind;
} ApplicationEvent_t;

void Application_OnButtonEvent(BTN_Buttons_e button, McuDbnc_EventKinds kind) {
  ApplicationEvent_t appEvent;
  appEvent.button = button;
  appEvent.kind = kind;
  xQueueSendToBack(eventQueueHandle,&appEvent,pdMS_TO_TICKS(100));
}

void queueTask(void *pv) {
  ApplicationEvent_t msgBuffer;
  
  for(;;) {
    if(xQueueReceive(eventQueueHandle,&msgBuffer,pdMS_TO_TICKS (20)) != pdPASS) { }
    switch(msgBuffer.button) {
      case BTN_NAV_CENTER:
        if(msgBuffer.kind == MCUDBNC_EVENT_PRESSED) {
          Leds_Neg(LEDS_GREEN);
          break;
        }
        if(msgBuffer.kind == MCUDBMC_STATE_LONG_PRESSED) {
          Leds_Off(LEDS_BLUE);
          Leds_Off(LEDS_RED);
          Leds_Off(LEDS_GREEN);
          break;
        }
        break;
      case BTN_NAV_UP:
        if(msgBuffer.kind == MCUDBNC_EVENT_PRESSED) {
          Leds_Neg(LEDS_RED);
        }
        break;
      case BTN_NAV_DOWN:
        if(msgBuffer.kind == MCUDBNC_EVENT_PRESSED) {
          Leds_Neg(LEDS_BLUE);
        }
        break;
      case BTN_NAV_LEFT:
        break;
      case BTN_NAV_RIGHT:
        break;
      default:
        printf("BTN %d does not exist!\n", (int)msgBuffer.button); // prints enum number of button
        break;
    }
    vTaskDelay(20);
  }
}

void Application_Init(void) {
  BaseType_t res;

  res = xTaskCreate(queueTask, "queueTask", 600/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+2, &queueTaskHandle);
  if (res!=pdPASS) {
    /* error! */
    for(;;) {}
  }

  eventQueueHandle = xQueueCreate(EVENT_QUEUE_LENGTH, EVENT_QUEUE_ELEM_SIZE);
  if(eventQueueHandle == NULL) {
    for(;;) { }
  }
  vQueueAddToRegistry(eventQueueHandle,"eventQueue");
}


void Application_Deinit() { }
 