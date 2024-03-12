/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "game.h"
#include "McuRTOS.h"
#include "McuWait.h"
#include "leds.h"
#include "sensor.h"
#include "Screensaver.h"

#if PL_CONFIG_USE_OLED_LCD
  #include "i2cbus.h"
  #include "McuSSD1306.h"
  #include "McuGDisplaySSD1306.h"
  #include "gameView.h"
  #include "gameModel.h"
  #include "McuFontDisplay.h"
  #include "McuGFont.h"
  #include "McuUtility.h"
  #include "McuFontCour08Normal.h"
  #include "McuFontHelv18Bold.h"
  #include "McuFontHelv12Bold.h"
  #include "McuFontHelv08Normal.h"
  #include "McuFontHelv12Normal.h"
#endif
#if PL_CONFIG_USE_INTEGRATION
  #include "boulder_images.h"
  #include "sprite.h"
  #include "gameModel.h"
  #include "gameView.h"
  #include "boulder.h"
#endif

#define CONFIG_GAME_PLAYING (1)

#define CONFIG_USE_QUEUE (1)
/* First idea: implementing very simple and crude synchronization between the game task
 *       and the debouncing engine. It works with a global variable, and the access is implemented in a reentrant way.
 * Second and better approach: using a FreeRTOS queue to properly synchronize between debouncing events and game task.
 */

// typedef struct
// {
// #if !CONFIG_USE_QUEUE
//   bool validEvent; /* used to mark if event is valid. Avoids double-reading */
// #endif
//   BTN_Buttons_e button;    /* button of the event */
//   McuDbnc_EventKinds kind; /* kind of button event */
// } Game_event_t;

#if PL_CONFIG_USE_GAME_EVENTS
  typedef enum Game_event_kind_e {
    Game_Event_Kind_None, /* ! < initialization value */
    Game_Event_Kind_Update_View, /* ! < request to update view */
    Game_Event_Kind_Game_Lost, /* ! < game is lost */
    Game_Event_Kind_Game_Won, /* ! < winning the game */
    Game_Event_Kind_Button, /* ! < button pressed */
    Game_Event_Kind_Score_Changed,
    } Game_event_kind_e;

typedef struct Game_event_t {
    Game_event_kind_e kind; /* kind of event . Data is inside union */
    union u_ {
        struct { /* for Game_Event_Kind_Button */
            BTN_Buttons_e button;
            McuDbnc_EventKinds kind;
        } btn;
    } u;
    uint16_t score_to_add;
} Game_event_t;
#else
typedef struct Game_event_t {
#if !PL_CONFIG_USE_QUEUE
  bool validEvent; /* used to mark if event is valid. Avoids double-reading */
#endif
  BTN_Buttons_e button; /* button of the event */
  McuDbnc_EventKinds kind; /* kind of button event */
} Game_event_t;
#endif

#if CONFIG_USE_QUEUE
static QueueHandle_t gameEventQueue;                      /* queue handle */
#define GAME_EVENT_QUEUE_LENGTH (32)                      /* number of elements in queue */
#define GAME_EVENT_QUEUE_ELEM_SIZE (sizeof(Game_event_t)) /* size of a queue item */
#else
static /*volatile*/ Game_event_t GameEvent;
#endif

static TimerHandle_t gameTimerHandle;
#define TIMER_PERIOD_MS (150)

#if CONFIG_USE_QUEUE
static void Game_SendEvent(Game_event_t *event) {
  if (xQueueSendToBack(gameEventQueue, event, pdMS_TO_TICKS(50)) != pdPASS) {
    /* failed to send event after timeout */
    for (;;) { }
  }
}
#endif

static uint8_t x_pos, y_pos;

void Game_OnScoreChanged(uint16_t score_to_add) {
  Game_event_t event;
  event.kind = Game_Event_Kind_Score_Changed;
  event.score_to_add = score_to_add;
  Game_SendEvent(&event);
}

void Game_OnButtonEvent(BTN_Buttons_e button, McuDbnc_EventKinds kind) {
#if PL_CONFIG_USE_GAME_EVENTS
  Game_event_t event;

  event.kind = Game_Event_Kind_Button;
  event.u.btn.button = button;
  event.u.btn.kind = kind;
  Game_SendEvent(&event);
#elif PL_CONFIG_USE_QUEUE
  Game_event_t event;

  event.button = button;
  event.kind = kind;
  Game_SendEvent(&event);
#else
  taskENTER_CRITICAL(); /* task critical section: events are created by timer task callback */
  GameEvent.validEvent = true;
  GameEvent.button = button;
  GameEvent.kind = kind;
  taskEXIT_CRITICAL();
#endif
}

#if 0 && PL_CONFIG_USE_OLED_LCD
static void DrawBox(void) {
  /* how to write to the display: */
  McuGDisplaySSD1306_Clear();
  McuGDisplaySSD1306_DrawBox(0, 0, McuGDisplaySSD1306_GetWidth()-1, McuGDisplaySSD1306_GetHeight()-1, 1, McuGDisplaySSD1306_COLOR_WHITE);
  McuGDisplaySSD1306_DrawCircle(20 /* x */, 30 /* y */, 5 /* radius */, McuGDisplaySSD1306_COLOR_WHITE);
  McuGDisplaySSD1306_UpdateFull();
}

static void DrawText(void) {
  /* how to use fonts: */
  uint8_t buf[24] = "hello";
  McuFontDisplay_PixelDim x = 0, y = 0;

  McuFontDisplay_WriteString(buf, McuGDisplaySSD1306_COLOR_WHITE, &x, &y, McuFontHelv08Normal_GetFont());
  McuGDisplaySSD1306_UpdateFull();
}

/* snippet to show temperature on display: */
void ShowTemperature(float value) {
  McuFontDisplay_PixelDim x, y, w, h;
  uint8_t buf[24];

  McuGDisplaySSD1306_Clear();
  McuUtility_NumFloatToStr(buf, sizeof(buf), value, 2);
  McuUtility_chcat(buf, sizeof(buf), 'C');
  w = McuFontDisplay_GetStringWidth(buf, McuFontHelv18Bold_GetFont(), NULL);
  h = McuFontDisplay_GetStringHeight(buf, McuFontHelv18Bold_GetFont(), NULL);
  x = (McuGDisplaySSD1306_GetWidth() - w) / 2;
  y = (McuGDisplaySSD1306_GetHeight() - h) / 2;
  McuFontDisplay_WriteString(buf, McuGDisplaySSD1306_COLOR_BLUE, &x, &y, McuFontHelv18Bold_GetFont());

  McuGDisplaySSD1306_DrawBox(0, 0, McuGDisplaySSD1306_GetWidth() - 1, McuGDisplaySSD1306_GetHeight() - 1, 1, McuGDisplaySSD1306_COLOR_WHITE);
  McuGDisplaySSD1306_DrawBox(2, 2, McuGDisplaySSD1306_GetWidth() - 5, McuGDisplaySSD1306_GetHeight() - 5, 1, McuGDisplaySSD1306_COLOR_WHITE);
  McuGDisplaySSD1306_UpdateFull();
}
#endif

static void gameTask(void *pv) {
  Game_event_t event;
  BaseType_t res;

  float temperature, humidity;
  int seed;

  temperature = GetTemp(); /* get values for random seed */
  humidity = GetHum(); /* get values for random seed */
  seed = (int)(temperature*10.0f) + (int)(humidity*10.0f);
  McuUtility_randomSetSeed(seed);

#if PL_CONFIG_USE_OLED_LCD
  I2CBus_Lock();
  McuSSD1306_Init();
  McuGDisplaySSD1306_Init();
  I2CBus_Unlock();

  // Game_ShowScreen(GAME_SCREEN_AEMBS);
  // vTaskDelay(pdMS_TO_TICKS(2000));
  // Game_ShowScreen(GAME_SCREEN_GRAPHICS);
  // vTaskDelay(pdMS_TO_TICKS(2000));
  // Game_ShowScreen(GAME_SCREEN_TEMPERATURE);
  // vTaskDelay(pdMS_TO_TICKS(2000));
#endif

#if PL_CONFIG_USE_CREATE_GAME_LEVEL
  Game_New();
  Game_ShowScreen(GAME_SCREEN_GAMEPLAY);
  if(xTimerStart(gameTimerHandle,portMAX_DELAY) != pdTRUE) {
    for(;;) { }
  }
#endif
  for (;;) {
    res = xQueueReceive(gameEventQueue, &event, portMAX_DELAY);
    if (res == pdTRUE) { /* message received */
      switch (event.kind) {
        case Game_Event_Kind_Button:
          switch (event.u.btn.button) {
            case BTN_NAV_UP:
              if (event.u.btn.kind == MCUDBNC_EVENT_PRESSED) {
                Game_PlayerAction(Game_Player_Move_Up, Game_Player_Speed_Normal);
              } else if (event.u.btn.kind==MCUDBNC_EVENT_PRESSED_REPEAT) {
                Game_PlayerAction(Game_Player_Move_Up, Game_Player_Speed_Fast);
              } else if (event.u.btn.kind==MCUDBNC_EVENT_LONG_PRESSED_REPEAT) {
                Game_PlayerAction(Game_Player_Move_Up, Game_Player_Speed_Fastest);
              }break;
            case BTN_NAV_DOWN:
              if (event.u.btn.kind==MCUDBNC_EVENT_PRESSED) {
                Game_PlayerAction(Game_Player_Move_Down, Game_Player_Speed_Normal);
              } else if (event.u.btn.kind==MCUDBNC_EVENT_PRESSED_REPEAT) {
                Game_PlayerAction(Game_Player_Move_Down, Game_Player_Speed_Fast);
              } else if (event.u.btn.kind==MCUDBNC_EVENT_LONG_PRESSED_REPEAT) {
                Game_PlayerAction(Game_Player_Move_Down, Game_Player_Speed_Fastest);
              }
              break;
            case BTN_NAV_LEFT:
              if (event.u.btn.kind==MCUDBNC_EVENT_PRESSED) {
                Game_PlayerAction(Game_Player_Move_Left, Game_Player_Speed_Normal);
              } else if (event.u.btn.kind==MCUDBNC_EVENT_PRESSED_REPEAT) {
                Game_PlayerAction(Game_Player_Move_Left, Game_Player_Speed_Fast);
              } else if (event.u.btn.kind==MCUDBNC_EVENT_LONG_PRESSED_REPEAT) {
                Game_PlayerAction(Game_Player_Move_Left, Game_Player_Speed_Fastest);
              }
              break;
            case BTN_NAV_RIGHT:
              if (event.u.btn.kind==MCUDBNC_EVENT_PRESSED) {
                Game_PlayerAction(Game_Player_Move_Right, Game_Player_Speed_Normal);
              } else if (event.u.btn.kind==MCUDBNC_EVENT_PRESSED_REPEAT) {
                Game_PlayerAction(Game_Player_Move_Right, Game_Player_Speed_Fast);
              } else if (event.u.btn.kind==MCUDBNC_EVENT_LONG_PRESSED_REPEAT) {
                Game_PlayerAction(Game_Player_Move_Right, Game_Player_Speed_Fastest);
              }
              break;
            case BTN_NAV_CENTER:
              break;
            default:
              break;
          } /* switch */
          break;
        case Game_Event_Kind_Update_View:
          Game_ShowScreen(GAME_SCREEN_GAMEPLAY);
          break;
        case Game_Event_Kind_Score_Changed:
          Game_SetScore(event.score_to_add);
          break;
        case Game_Event_Kind_Game_Won:
          ScreenSaver_StopTimer();
          Game_ShowScreen(GAME_SCREEN_GAME_WON);
          vTaskDelay(pdMS_TO_TICKS(3000));
          Game_ShowScreen(GAME_SCREEN_SCORE);
          vTaskDelay(pdMS_TO_TICKS(3000));
          if(xTimerStart(gameTimerHandle,portMAX_DELAY) != pdTRUE) {
            for(;;) { }
          }
          Game_IncreaseLevel();
          Game_NextLevel(Game_GetLevel());
          Game_ShowScreen(GAME_SCREEN_GAMEPLAY);
          ScreenSaver_StartTimer();
          break;
        case Game_Event_Kind_Game_Lost:
          ScreenSaver_StopTimer();
          Game_ShowScreen(GAME_SCREEN_GAME_LOST);
          vTaskDelay(pdMS_TO_TICKS(3000));
          Game_ShowScreen(GAME_SCREEN_SCORE);
          vTaskDelay(pdMS_TO_TICKS(3000));
          if(xTimerStart(gameTimerHandle,portMAX_DELAY) != pdTRUE) {
            for(;;) { }
          }
          Game_New();
          Game_ShowScreen(GAME_SCREEN_GAMEPLAY);
          ScreenSaver_StartTimer();
          break;
        default:
          break;
      } /* if */
    }   /* for */
    Leds_Neg(LEDS_GREEN);
  }
}

// void Game_GetCoordinates(uint8_t *x, uint8_t *y)
// {
//   taskENTER_CRITICAL();
//   *x = x_pos;
//   *y = y_pos;
//   taskEXIT_CRITICAL();
//   return;
// }

void Game_Deinit(void) {
#if CONFIG_USE_QUEUE
  vQueueDelete(gameEventQueue);
  gameEventQueue = NULL;
#endif
}

void Game_Init(void) {
  BaseType_t res;

  res = xTaskCreate(gameTask, "gameTask", 600 / sizeof(StackType_t), NULL, tskIDLE_PRIORITY + 2, NULL);
  if (res != pdPASS) {
    /* error! */
    for (;;)
    {
    }
  }
#if CONFIG_USE_QUEUE
  gameEventQueue = xQueueCreate(GAME_EVENT_QUEUE_LENGTH, GAME_EVENT_QUEUE_ELEM_SIZE);
  if (gameEventQueue == NULL) {
    for (;;) { } /* out of memory? */
  }
  vQueueAddToRegistry(gameEventQueue, "GameEventQueue");
#endif
  
  gameTimerHandle = xTimerCreate(
        "gameTimer", /* name */
        pdMS_TO_TICKS(TIMER_PERIOD_MS), /* period/time */
        pdTRUE, /* auto reload */
        (void*)0, /* timer ID */
        vTimerCallbackGame); /* callback */
  if (gameTimerHandle==NULL) {
    for(;;); /* failure! */
  }
}

void Game_BeginScreensaver(void) {
  Game_TimerStop();
  vTaskDelay(pdMS_TO_TICKS(20));
  Game_ShowScreen(GAME_SCREEN_SCREENSAVER);
}

void Game_EndScreensaver(void) {
  Game_ShowScreen(GAME_SCREEN_END_SCREENSAVER);
  vTaskDelay(pdMS_TO_TICKS(1000));
  Game_TimerStart();
}

void Game_TimerStart(void) {
  xTimerStart(gameTimerHandle,pdMS_TO_TICKS(100));
}

void Game_TimerStop(void) {
  xTimerStop(gameTimerHandle,pdMS_TO_TICKS(100));
}

static void vTimerCallbackGame(TimerHandle_t gameHandle) {
  Game_event_t event;
  Game_AgeAndCheckIfAlive();
  Game_HandleFallingItems();
  Game_CheckCollisions();
  Game_HandleMonsterMoving();

  if(Game_CheckIfGameWon()) {
    event.kind = Game_Event_Kind_Game_Won;
    xTimerStop(gameTimerHandle,pdMS_TO_TICKS(100));
  } else if(Game_CheckIfGameLost()) {
    event.kind = Game_Event_Kind_Game_Lost;
    xTimerStop(gameTimerHandle,pdMS_TO_TICKS(100));
  } else event.kind = Game_Event_Kind_Update_View;
  Game_SendEvent(&event);
}