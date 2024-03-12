/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "McuRTOS.h"
#include "McuSystemView.h"

#include <stdint.h>
#include <stdio.h>

#include "myTasks.h"
#include "leds.h"

static uint32_t taskParam1 = LEDS_BLUE;
static uint32_t taskParam2 = LEDS_RED;
static TaskHandle_t queueTaskHandle = NULL;
static TaskHandle_t killmeTaskHandle = NULL;

static void killmeTask(void *pv) {
  vTaskPrioritySet(NULL, 5);
  vTaskSuspend(NULL);
  vTaskDelete(NULL);
}

static void otherTask(void *pv) {
  vTaskResume(killmeTaskHandle);
  vTaskDelete(NULL);
}

static void mainTask(void *pv) {
  LEDS_Leds_e led = *(uint32_t*)pv;

  printf("mainTask led param is %d\n", (int)led);
  /* testing task priorities */
  vTaskPrioritySet(NULL, uxTaskPriorityGet(NULL)+1);
  vTaskPrioritySet(NULL, uxTaskPriorityGet(NULL)-1);
  for(;;) {
    McuSystemView_Print((const char*)"Blink!\r\n");
    vTaskDelay(pdMS_TO_TICKS(500));
    McuSystemView_OnUserStart(1);
    Leds_Neg(led);
    McuSystemView_OnUserStop(1);
  }
}

void MyTasks_Deinit(void) {
}

void MyTasks_Init(void) {
  BaseType_t res;

  res = xTaskCreate(mainTask, "mainTask", 600/sizeof(StackType_t), &taskParam1, tskIDLE_PRIORITY+2, &queueTaskHandle);
  if (res!=pdPASS) {
    /* error! */
    for(;;) {}
  }
  /* second task with same task code */
  res = xTaskCreate(mainTask, "myTask2", 600/sizeof(StackType_t), &taskParam2, tskIDLE_PRIORITY+1, NULL);
  if (res!=pdPASS) {
    for(;;) {} /* error! */
  }
  res = xTaskCreate(killmeTask, "killmeTask2", 600/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+3, &killmeTaskHandle);
  if (res!=pdPASS) {
    for(;;) {} /* error! */
  }
  res = xTaskCreate(otherTask, "otherTask", 600/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+3, NULL);
  if (res!=pdPASS) {
    for(;;) {} /* error! */
  }
}
