/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SCREENSAVER_H_
#define SCREENSAVER_H_

/*!
 * \brief Callback, called for every button press to reset the screensaver
 */
void Screensaver_OnButtonPress(void);

/*!
 * \brief Start the screensaver timer
 */
void ScreenSaver_StartTimer(void);

/*!
 * \brief Stop the screensaver timer
 */
void ScreenSaver_StopTimer(void);

/*!
 * \brief Module de-initialization
 */
void Screensaver_Deinit(void);

/*!
 * \brief Module initialization
 */
void Screensaver_Init(void);

#endif /* SCREENSAVER_H_ */
