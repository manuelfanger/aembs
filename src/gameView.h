/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef GAMEVIEW_H_
#define GAMEVIEW_H_

#include "platform.h"
#include "game_config.h"

typedef enum Game_Screen_e {
  GAME_SCREEN_AEMBS,  /* show screen with module name */
  GAME_SCREEN_GRAPHICS, /*! \todo */
  GAME_SCREEN_TEMPERATURE, /*! \todo */
  GAME_SCREEN_GAMEPLAY, /* update gameplay screen and show all elements */
  GAME_SCREEN_RECTANGLES,
  GAME_SCREEN_GAME,
  GAME_SCREEN_SCORE,
  GAME_SCREEN_GAME_LOST,
  GAME_SCREEN_GAME_WON,
  GAME_SCREEN_SCREENSAVER,
  GAME_SCREEN_END_SCREENSAVER,
  /*! \todo add more items as needed */
} Game_Screen_e;

/*!
 * \brief Show a screen on the display
 * \param screen Game screen to be shown
 */
void Game_ShowScreen(Game_Screen_e screen);

/*!
 * \brief View de-initialization
 */
void Game_ViewDeinit(void);

/*!
 * \brief View initialization
 */
void Game_ViewInit(void);

#endif /* GAMEVIEW_H_ */
