/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef GAME_H_
#define GAME_H_

#include "buttons.h"
#include "McuDebounce.h"

void Game_OnScoreChanged(uint16_t score);

/*!
 * \brief Event handler for the game, dealing with button events. Events called from debouncing.
 * \param button Button, for which the event applies
 * \param kind Kind of button event
 */
void Game_OnButtonEvent(BTN_Buttons_e button, McuDbnc_EventKinds kind);

void Game_BeginScreensaver(void);

void Game_EndScreensaver(void);

void Game_TimerStart(void);

void Game_TimerStop(void);

/*!
 * \brief Module initialization
 */
void Game_Deinit(void);

/*!
 * \brief Module de-initialization
 */
void Game_Init(void);

void Game_GetCoordinates(uint8_t *x, uint8_t *y);

static void vTimerCallbackGame(TimerHandle_t gameHandle);

#endif /* GAME_H_ */
