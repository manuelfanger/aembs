/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "game_config.h"
#include "game.h"
#include "gameView.h"
#include "gameModel.h"
#include "sprite.h"
#include "McuUtility.h"
#include "McuGDisplaySSD1306.h"
#include "McuFontDisplay.h"
#include "McuFontHelv10Normal.h"
#include "McuFontHelv12Normal.h"
#include "McuFontHelv18Bold.h"
#if configUSE_SEGGER_SYSTEM_VIEWER_HOOKS
  #include "McuSystemView.h"
#endif
#include "boulder.h"
#include "boulder_images.h"

#if PL_CONFIG_USE_OLED_LCD
  #include "sensor.h"
  #include "i2cbus.h"
#endif

/*!
 * \brief Shows one line of text centered on the display, with a border around it
 * \param text0 Line one of text
 */
static void Show1Liner(const unsigned char *text0) {
  /*! \todo Make sure things are reentrant! */
  PGFONT_Callbacks font = McuFontHelv12Normal_GetFont();
  McuFontDisplay_PixelDim x, y;
  McuFontDisplay_PixelDim charHeight, totalHeight;

  McuGDisplaySSD1306_Clear();
  McuFontDisplay_GetFontHeight(font, &charHeight, &totalHeight);

  x = McuGDisplaySSD1306_GetWidth()/2 - McuFontDisplay_GetStringWidth((unsigned char*)text0, font, NULL)/2;
  y = McuGDisplaySSD1306_GetHeight()/2 - charHeight/2; /* 1 line */
  McuFontDisplay_WriteString((unsigned char*)text0, McuGDisplaySSD1306_COLOR_BLUE, &x, &y, font);

  McuGDisplaySSD1306_DrawBox(0, 0, McuGDisplaySSD1306_GetWidth()-1, McuGDisplaySSD1306_GetHeight()-1, 1, McuGDisplaySSD1306_COLOR_BLUE);
  McuGDisplaySSD1306_DrawBox(2, 2, McuGDisplaySSD1306_GetWidth()-1-4, McuGDisplaySSD1306_GetHeight()-1-4, 1, McuGDisplaySSD1306_COLOR_BLUE);
#if PL_CONFIG_USE_OLED_LCD
  I2CBus_Lock();
#endif
  McuGDisplaySSD1306_UpdateFull();
#if PL_CONFIG_USE_OLED_LCD
  I2CBus_Unlock();
#endif
}

/*!
 * \brief Shows two lines of text centered on the display, with a border around it
 * \param text0 Line one of text
 * \param text1 Line two of text
 */
static void Show2Liner(const unsigned char *text0, const unsigned char *text1) {
  /*! \todo Make sure things are reentrant! */
  PGFONT_Callbacks font = McuFontHelv12Normal_GetFont();
  McuFontDisplay_PixelDim x, y;
  McuFontDisplay_PixelDim charHeight, totalHeight;

  McuGDisplaySSD1306_Clear();
  McuFontDisplay_GetFontHeight(font, &charHeight, &totalHeight);

  x = McuGDisplaySSD1306_GetWidth()/2 - McuFontDisplay_GetStringWidth((unsigned char*)text0, font, NULL)/2;
  y = McuGDisplaySSD1306_GetHeight()/2 - (2*charHeight)/2; /* 2 lines */
  McuFontDisplay_WriteString((unsigned char*)text0, McuGDisplaySSD1306_COLOR_BLUE, &x, &y, font);

  x = McuGDisplaySSD1306_GetWidth()/2 - McuFontDisplay_GetStringWidth((unsigned char*)text1, font, NULL)/2;
  y += McuGDisplaySSD1306_GetHeight()/2 - (2*charHeight)/2;
  McuFontDisplay_WriteString((unsigned char*)text1, McuGDisplaySSD1306_COLOR_BLUE, &x, &y, font);

  McuGDisplaySSD1306_DrawBox(0, 0, McuGDisplaySSD1306_GetWidth()-1, McuGDisplaySSD1306_GetHeight()-1, 1, McuGDisplaySSD1306_COLOR_BLUE);
  McuGDisplaySSD1306_DrawBox(2, 2, McuGDisplaySSD1306_GetWidth()-1-4, McuGDisplaySSD1306_GetHeight()-1-4, 1, McuGDisplaySSD1306_COLOR_BLUE);
#if PL_CONFIG_USE_OLED_LCD
  I2CBus_Lock();
#endif
  McuGDisplaySSD1306_UpdateFull();
#if PL_CONFIG_USE_OLED_LCD
  I2CBus_Unlock();
#endif
}

static void PaintGamePlayScreen(void) {
  /*! \todo Make sure things are reentrant! */
  McuGDisplaySSD1306_Clear();
  Boulder_PaintTiles();
  int cntr = 0;
  Sprite_t **sprites;

  do {
    sprites = Game_IteratorGetSpriteLists(&cntr);
    if (sprites!=NULL) {
      Sprite_PaintList(*sprites);
    }
  } while(cntr!=0);
#if PL_CONFIG_USE_OLED_LCD
  I2CBus_Lock();
#endif
  McuGDisplaySSD1306_UpdateFull();
#if PL_CONFIG_USE_OLED_LCD
  I2CBus_Unlock();
#endif
}

#if PL_CONFIG_USE_OLED_LCD

static void PaintGraphics(void) {
  McuGDisplaySSD1306_Clear();
  McuGDisplaySSD1306_DrawBox(0, 0, McuGDisplaySSD1306_GetWidth()-1, McuGDisplaySSD1306_GetHeight()-1, 1, McuGDisplaySSD1306_COLOR_WHITE);
  McuGDisplaySSD1306_DrawCircle(20 /* x */, 30 /* y */, 5 /* radius */, McuGDisplaySSD1306_COLOR_WHITE);
  McuGDisplaySSD1306_DrawCircle(50 /* x */, 20 /* y */, 15 /* radius */, McuGDisplaySSD1306_COLOR_WHITE);
  McuGDisplaySSD1306_DrawLine(McuGDisplaySSD1306_GetWidth()-1, McuGDisplaySSD1306_GetHeight()-1, 0, 0, McuGDisplaySSD1306_COLOR_WHITE);
  McuGDisplaySSD1306_DrawBox(25, 25, 30, 30, 2, McuGDisplaySSD1306_COLOR_WHITE);
  McuGDisplaySSD1306_DrawLine(50, 10, 60, 20, McuGDisplaySSD1306_COLOR_WHITE);
  McuGDisplaySSD1306_DrawLine(60, 20, 40, 20, McuGDisplaySSD1306_COLOR_WHITE);
  McuGDisplaySSD1306_DrawLine(40, 20, 50, 10, McuGDisplaySSD1306_COLOR_WHITE);
  I2CBus_Lock();
  McuGDisplaySSD1306_UpdateFull();
  I2CBus_Unlock();
}

static void PaintTemperature(float value) {
  /*! \todo show temperature on display. Make sure things are reentrant! */
  McuFontDisplay_PixelDim x, y, w, h;
  uint8_t buf[24];

  McuGDisplaySSD1306_Clear();
  McuUtility_NumFloatToStr(buf,  sizeof(buf), value, 2);
  McuUtility_chcat(buf, sizeof(buf), 'C');
  w = McuFontDisplay_GetStringWidth(buf, McuFontHelv18Bold_GetFont(), NULL);
  h = McuFontDisplay_GetStringHeight(buf, McuFontHelv18Bold_GetFont(), NULL);
  x = (McuGDisplaySSD1306_GetWidth()-w)/2;
  y = (McuGDisplaySSD1306_GetHeight()-h)/2;
  McuFontDisplay_WriteString(buf, McuGDisplaySSD1306_COLOR_BLUE, &x, &y, McuFontHelv18Bold_GetFont());

  McuGDisplaySSD1306_DrawBox(0, 0, McuGDisplaySSD1306_GetWidth()-1, McuGDisplaySSD1306_GetHeight()-1, 1, McuGDisplaySSD1306_COLOR_WHITE);
  McuGDisplaySSD1306_DrawBox(2, 2, McuGDisplaySSD1306_GetWidth()-5, McuGDisplaySSD1306_GetHeight()-5, 1, McuGDisplaySSD1306_COLOR_WHITE);
  I2CBus_Lock();
  McuGDisplaySSD1306_UpdateFull();
  I2CBus_Unlock();
}

static void PaintRectangles(void) {
  McuGDisplaySSD1306_Clear();
  McuGDisplaySSD1306_DrawBox(0, 0, McuGDisplaySSD1306_GetWidth()-1, McuGDisplaySSD1306_GetHeight()-1, 1, McuGDisplaySSD1306_COLOR_WHITE);
  McuGDisplaySSD1306_DrawBox(25, 25, 30, 30, 2, McuGDisplaySSD1306_COLOR_WHITE);
  McuGDisplaySSD1306_DrawBox(5, 15, 20, 10, 2, McuGDisplaySSD1306_COLOR_WHITE);
  McuGDisplaySSD1306_DrawBox(10, 5, 35, 25, 2, McuGDisplaySSD1306_COLOR_WHITE);
  McuGDisplaySSD1306_DrawBox(45, 15, 60, 40, 2, McuGDisplaySSD1306_COLOR_WHITE);

  I2CBus_Lock();
  McuGDisplaySSD1306_UpdateFull();
  I2CBus_Unlock();
}

static void PaintScore(int score) {
  uint8_t buf[12];
  McuGDisplaySSD1306_Clear();
  McuUtility_Num16uToStr(buf,  sizeof(buf), score);
  Show2Liner((const unsigned char*)"Score:", (const unsigned char*)buf);
  I2CBus_Lock();
  McuGDisplaySSD1306_UpdateFull();
  I2CBus_Unlock();
}

static void PaintScreensaver(void) {
  McuGDisplaySSD1306_Clear();
  Show1Liner("d[-_-]b");
  I2CBus_Lock();
  McuGDisplaySSD1306_UpdateFull();
  I2CBus_Unlock();
}

static void PaintEndScreensaver(void) {
  McuGDisplaySSD1306_Clear();
  Show1Liner("d[0 o 0]b");
  I2CBus_Lock();
  McuGDisplaySSD1306_UpdateFull();
  I2CBus_Unlock();
}

// static void PaintGame(uint8_t x, uint8_t y) {
//   McuGDisplaySSD1306_Clear();
//   McuGDisplaySSD1306_DrawBox(0, 0, McuGDisplaySSD1306_GetWidth()-1, McuGDisplaySSD1306_GetHeight()-1, 1, McuGDisplaySSD1306_COLOR_WHITE);
//   McuGDisplaySSD1306_DrawBox(x, y, 10, 10, 2, McuGDisplaySSD1306_COLOR_WHITE);

//   I2CBus_Lock();
//   McuGDisplaySSD1306_UpdateFull();
//   I2CBus_Unlock();
// }
#endif

void Game_ShowScreen(Game_Screen_e screen) {
  switch(screen) {
    /*! \todo extend as needed */
    case GAME_SCREEN_AEMBS:
      Show1Liner((const unsigned char*)"AEMBS");
      break;
    case GAME_SCREEN_GRAPHICS: /*! \todo */
      Show2Liner((const unsigned char*)"Some", (const unsigned char*)"Graphics"); /*! \todo test display with graphics */
#if PL_CONFIG_USE_OLED_LCD
      PaintGraphics();
#endif
      break;
    case GAME_SCREEN_TEMPERATURE: /*! \todo */
#if PL_CONFIG_USE_OLED_LCD
      PaintTemperature(GetTemp());
#endif
      break;
//     case GAME_SCREEN_GAME:
// #if PL_CONFIG_USE_OLED_LCD
//       uint8_t x, y;
//       Game_GetCoordinates(&x, &y);
//       PaintGame(x, y);
// #endif
//       break;
    case GAME_SCREEN_GAMEPLAY:
      PaintGamePlayScreen();
      break;
    case GAME_SCREEN_SCORE:
      PaintScore(Game_GetScore());
      break;
    case GAME_SCREEN_GAME_WON:
      Show2Liner((const unsigned char*)"You did it!", (const unsigned char*)":-)");
      break;
    case GAME_SCREEN_GAME_LOST:
      Show2Liner((const unsigned char*)"You are dead!", (const unsigned char*)":-(");
      break;
    case GAME_SCREEN_SCREENSAVER:
      PaintScreensaver();
      break;
    case GAME_SCREEN_END_SCREENSAVER:
      PaintEndScreensaver();
      break;
    default:
      break;
  }
}

void Game_ViewInit(void) {
  /* nothing needed */
}

void Game_ViewDeinit(void) {
  /* nothing needed */
}
