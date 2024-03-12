/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Screensaver.h"

#include "game.h"

#include "McuRTOS.h"

#define SCREEN_SAVER_CONFIG_EXPIRE_MS    (5000) /*!< timeout in number of milli-seconds */

static TimerHandle_t timerHandle;
static bool screensaverActive = FALSE;

void ScreenSaver_StartTimer(void) {
  xTimerStart(timerHandle,0);
}

void ScreenSaver_StopTimer(void) {
  xTimerStop(timerHandle,0);
}

void Screensaver_OnButtonPress(void) {
  xTimerReset(timerHandle,0);
  if(screensaverActive == TRUE) {
    Game_EndScreensaver();
    screensaverActive = FALSE;
  }
}

static void vTimerExpiredCallback(TimerHandle_t pxTimer) {
  Game_BeginScreensaver();
  screensaverActive = TRUE;
}

void Screensaver_Deinit(void) {
  ScreenSaver_StopTimer();
}

void Screensaver_Init(void) {
  timerHandle = xTimerCreate("Screensaver Timer",pdMS_TO_TICKS(SCREEN_SAVER_CONFIG_EXPIRE_MS),pdFALSE,0,vTimerExpiredCallback);
  if(timerHandle == NULL) {
    for(;;) { }
  }
  ScreenSaver_StartTimer();
}
