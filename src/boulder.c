/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "boulder.h"
#include <string.h>
#include "boulder_images.h"
#include "game_config.h"
#include "game.h"
#include "gameModel.h"

/* different boulder tile items */
#define DM   Boulder_Tile_Diamond   /* DM */
#define DI   Boulder_Tile_Dirt      /* DI */
#define BO   Boulder_Tile_Boulder   /* BO */
#define WA   Boulder_Tile_Wall      /* WA */
#define EE   Boulder_Tile_Empty     /* EE */
#define PL   Boulder_Tile_Player    /* PL */
#define MO   Boulder_Tile_Monster   /* MO */

static const uint8_t Boulder_levels[GAME_CONFIG_NOF_LEVEL][BOULDER_TILE_NOF_Y][BOULDER_TILE_NOF_X] =
{
#if 0 /* for tests only */
  {
   {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI},
   {DI,DM,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI},
   {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,BO,PL,DI,DI},
   {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,EE,DI,DI,DI},
   {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,EE,DI,DI,DI},
   {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,EE,DI,DI,DI},
   {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,MO,DI,DI,DI},
   {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI}
  },
#else
  {
   {DI,BO,DI,DI,DI,DI,DI,DI,DI,BO,BO,DI,DI,DI,DI,DI},
   {DI,DM,DM,DM,DI,DI,DI,DI,DI,DI,DI,DI,DI,BO,DI,DI},
   {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DM,DM,DM,DI,DI},
   {DI,DI,BO,DI,DI,DI,DI,DI,DI,DI,BO,DI,DI,DI,DI,DI},
   {DI,DI,BO,EE,MO,EE,BO,PL,DI,EE,BO,DI,DI,DI,DI,DI},
   {DI,DI,DI,BO,EE,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI},
   {DI,BO,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,BO,BO,DI,DI},
   {DI,DM,DM,DM,DI,DI,DI,DI,DI,DI,DI,DM,DM,DM,DI,DI}
  },
#endif
#if GAME_CONFIG_NOF_LEVEL>1
  {
   {DI,BO,BO,BO,BO,DI,DI,DI,DI,DI,BO,DI,DI,DI,DI,DI},
   {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI},
   {DI,DM,DM,DM,DM,DI,DI,WA,DI,DI,DM,DM,DM,DI,DI,DI},
   {DI,DI,DI,DI,DI,DI,DI,WA,DI,DI,DI,DI,DI,DI,DI,DI},
   {DI,DI,DI,DI,DI,DI,DI,WA,DI,DI,DI,DI,DI,DI,DI,DI},
   {DI,DI,DI,DI,BO,DI,DI,WA,DI,DI,DI,DM,DM,DM,DI,DI},
   {DI,DI,DI,DI,DM,DI,DI,WA,DI,DI,DI,BO,BO,BO,DI,DI},
   {DI,PL,DI,DI,DM,DI,DI,WA,DI,DI,DI,DI,DI,DI,DI,DI}
  },
#endif
#if GAME_CONFIG_NOF_LEVEL>2
  {
   {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,BO,DI,DI,DI,DI,DI},
   {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DM,DI,DI,DI,DI,DI},
   {DI,DI,DI,DI,DI,EE,MO,EE,DI,DI,DM,DI,DI,DI,DI,DI},
   {DI,DI,BO,DI,DI,DI,DI,DI,DI,DI,DM,DI,DI,DI,DI,DI},
   {DI,DI,DI,DI,DI,DI,DI,WA,DI,DI,DI,DI,DI,DI,DI,DI},
   {DI,DI,DM,DI,DI,DI,DI,WA,DI,DI,DI,DI,DI,DI,DI,DI},
   {DI,DI,DM,DI,DI,DI,DI,WA,WA,WA,WA,WA,WA,WA,WA,WA},
   {DI,DI,DM,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,PL,DI,DI}
  },
#endif
#if GAME_CONFIG_NOF_LEVEL>3
  {
   {DI,DI,DI,BO,BO,BO,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI},
   {DI,DI,DI,DI,DI,DI,DI,DI,DI,DM,DM,DM,DI,DI,DI,DI},
   {DI,DI,DI,EE,MO,EE,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI},
   {DI,DI,DI,DM,DM,DM,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI},
   {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI},
   {DI,DI,DI,BO,BO,BO,DI,DI,DI,DI,DI,DI,DI,PL,DI,DI},
   {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI},
   {DI,DI,DI,DM,DM,DM,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI}
  },
#endif
#if GAME_CONFIG_NOF_LEVEL>4
  {
    {DI,DI,DI,BO,BO,BO,BO,DI,DI,DI,WA,DI,DI,DI,DI,DI},
    {BO,DI,DI,DM,DM,DM,DM,DI,DI,DI,WA,DI,DI,BO,BO,DI},
    {DM,DI,DI,DI,DI,DI,DI,DI,DI,DI,WA,DI,DI,DM,DM,DI},
    {DM,DI,DI,DI,WA,DI,DI,DI,DI,DI,WA,BO,BO,DI,DI,DI},
    {DM,DI,DI,BO,WA,DI,DI,DI,DI,DI,WA,DM,DM,DI,DI,DI},
    {DI,DI,DI,DM,WA,BO,BO,DI,DI,DI,WA,DI,DI,DI,DI,DI},
    {DI,DI,DI,DM,WA,DI,DI,EE,DI,DI,WA,WA,WA,WA,DI,DI},
    {DI,PL,DI,DI,WA,EE,EE,MO,DI,DI,DM,DM,DM,DM,DI,DI}
  },
#endif
#if GAME_CONFIG_NOF_LEVEL>5
  {
     {PL,DI,DI,DI,DI,DM,DM,DM,DI,DI,DI,DI,DI,DI,DI,BO},
     {DI,DI,DI,DI,DI,DM,DM,DM,DI,DI,DI,DI,DI,DM,DM,DM},
     {WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,DI,DI,DI,DI},
     {BO,BO,DI,DI,DM,DM,DM,DM,DI,DI,DI,DI,DI,DI,BO,BO},
     {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DM,DM},
     {DI,DI,DI,DI,DI,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA},
     {DI,DI,DI,DI,DI,DM,DM,DM,DI,DI,DI,DI,WA,DI,DI,DI},
     {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI}
   },
#endif
#if GAME_CONFIG_NOF_LEVEL>6
   {
     {DI,DI,DI,DI,DI,BO,DM,DM,DM,DM,DI,DI,DI,DI,BO,BO},
     {DI,DI,DI,BO,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DM,DM},
     {DI,DI,DI,DM,WA,WA,WA,WA,WA,WA,WA,BO,DI,DI,DI,DI},
     {DI,DI,DI,DM,WA,DI,DI,DI,BO,BO,WA,DM,DI,DI,DI,DI},
     {DI,DI,DI,DM,WA,DM,DM,DM,DM,DM,WA,DM,DI,DI,DI,DI},
     {MO,DI,DI,DI,WA,DI,DI,DI,DI,DI,WA,DM,DI,DI,DI,MO},
     {EE,BO,BO,DI,WA,WA,WA,DI,WA,WA,WA,DI,BO,BO,BO,EE},
     {DI,EE,EE,DI,DI,DI,DI,PL,DI,DI,DI,DI,EE,EE,EE,DI}
   },
#endif
#if GAME_CONFIG_NOF_LEVEL>7
   {
     {DI,BO,BO,BO,DI,DI,DI,DI,DI,BO,BO,DI,DI,BO,BO,BO},
     {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI},
     {DI,EE,MO,EE,DI,DI,DI,DI,DI,DI,DI,DI,DI,EE,MO,EE},
     {DI,DM,DM,DM,DI,DI,BO,BO,BO,BO,BO,DI,DI,DM,DM,DM},
     {DI,WA,WA,WA,WA,DI,DM,DM,DM,DM,DM,DI,WA,WA,WA,WA},
     {DI,WA,BO,BO,WA,DI,DI,DI,DI,DI,DI,DI,WA,BO,BO,WA},
     {DI,WA,WA,WA,WA,DI,DI,DI,PL,DI,DI,DI,WA,WA,WA,WA},
     {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI}
   },
#endif
#if GAME_CONFIG_NOF_LEVEL>8
   {
     {DI,DI,DI,DI,DI,DI,DI,DI,BO,BO,DI,BO,DI,DI,DI,DI},
     {DI,DI,DI,BO,DI,DI,DI,DI,DM,DM,DI,DI,DI,DI,DI,BO},
     {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,EE,DI,DI,DI,DM},
     {BO,BO,DI,EE,DI,DI,DI,BO,DI,DI,DI,EE,DI,DI,DI,DI},
     {DM,DM,DI,MO,DI,DI,DI,DI,DI,DI,DI,MO,DI,BO,BO,DI},
     {DM,DM,DI,EE,DI,DI,DI,EE,DI,DI,DI,DI,DI,DM,DM,DI},
     {DI,DI,DI,DI,DI,DI,DI,EE,DI,DI,DI,BO,BO,DI,DI,DI},
     {DI,DI,DI,DI,DI,DI,DI,EE,DI,DI,DM,DM,DM,DI,DI,PL}
   },
#endif
#if GAME_CONFIG_NOF_LEVEL>9
   {
     {DI,DI,BO,BO,BO,DI,WA,DI,BO,DI,WA,DI,DI,BO,BO,BO},
     {DM,DM,DM,DM,DM,DI,WA,DI,DM,DI,WA,DM,DM,DM,DM,DM},
     {DI,DI,DI,DI,DI,DI,WA,DI,DM,DI,WA,DI,DI,DI,DI,DI},
     {WA,WA,WA,WA,WA,DI,WA,DI,DM,DI,WA,DI,WA,WA,WA,WA},
     {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI},
     {DI,BO,BO,DI,DI,DI,PL,DI,DI,DI,WA,DI,WA,WA,WA,WA},
     {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,WA,DI,DM,DM,DM,DM},
     {DI,DI,DI,DI,DI,EE,MO,EE,DI,DI,WA,DI,DI,DI,DI,DI}
   },
#endif
#if GAME_CONFIG_NOF_LEVEL>10
   {
     {PL,DI,DI,BO,BO,BO,DI,BO,BO,BO,BO,DI,DM,DM,DI,DI},
     {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI},
     {DI,DI,BO,EE,MO,EE,BO,EE,MO,EE,EE,BO,DI,DI,DI,DI},
     {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI},
     {DI,DI,MO,DI,DM,DI,MO,DI,DM,DI,DI,MO,DI,DI,BO,BO},
     {DI,DI,EE,DI,DM,DI,EE,DI,DM,DI,DI,EE,DI,DI,DM,DM},
     {DI,DI,EE,DI,DM,DI,EE,DI,DM,DI,DI,EE,DI,DI,DM,DM},
     {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI}
   },
#endif
#if GAME_CONFIG_NOF_LEVEL>11
   {
     {DM,DM,DM,DI,WA,BO,BO,BO,BO,BO,BO,WA,DI,DM,DM,DM},
     {DI,BO,DI,DI,WA,DM,DM,DM,DM,DM,DM,WA,DI,DI,BO,DI},
     {DI,DI,DI,DI,WA,DI,DM,DM,DM,DM,DI,WA,DI,DI,DI,DI},
     {DI,MO,DI,DI,WA,DI,DI,DI,DI,DI,DI,WA,DI,DI,MO,DI},
     {DI,EE,DI,DI,WA,WA,WA,DI,DI,WA,WA,WA,DI,DI,EE,DI},
     {DI,EE,DI,DI,DM,DM,DI,DI,DI,DM,DM,DI,DI,DI,EE,DI},
     {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI},
     {DI,DI,DI,DI,DI,PL,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI}
   },
#endif
#if GAME_CONFIG_NOF_LEVEL>12
   {
     {DI,DI,DI,DI,DI,DI,BO,BO,DM,DM,DI,DI,DI,BO,DI,DI},
     {DI,DI,DI,DI,DI,DI,DM,DM,DI,DI,DI,DI,DI,DI,DI,DI},
     {WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,DI,DI,DI,EE,DI,DI},
     {DI,BO,DI,BO,DI,DI,DI,DM,DM,DM,DM,DI,DI,MO,DI,DI},
     {DI,DI,DI,DI,DI,DI,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA},
     {DI,MO,DI,MO,DI,DI,BO,BO,BO,BO,BO,BO,BO,BO,BO,BO},
     {DI,EE,DI,EE,DI,DI,DM,DM,DM,DM,DM,DM,DM,DM,DI,DI},
     {DI,EE,DI,EE,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,PL}
   },
#endif
#if GAME_CONFIG_NOF_LEVEL>13
   {
     {DI,DI,DI,DI,WA,DI,DM,DM,DM,DI,DI,DI,DI,WA,BO,BO},
     {WA,WA,WA,WA,WA,DI,DI,DI,DI,DI,DI,DI,DI,WA,WA,WA},
     {DM,DM,DI,DI,DI,DI,EE,DI,EE,DI,DI,DI,DI,DI,DI,DM},
     {DM,DM,DI,DI,DI,DI,EE,DI,EE,DI,DI,DI,DI,DI,DI,DM},
     {WA,WA,WA,WA,DI,DI,MO,DI,MO,DI,DI,DI,WA,WA,WA,WA},
     {BO,BO,BO,WA,DI,DI,DI,DI,DI,DI,DI,DI,WA,DI,BO,BO},
     {DM,DM,DM,WA,DI,DI,DI,DI,DI,DI,DI,DI,WA,DI,DM,DM},
     {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,PL}
   },
#endif
#if GAME_CONFIG_NOF_LEVEL>14
   {
     {BO,BO,DI,DI,DI,DI,DI,PL,DI,DI,DI,DI,DI,BO,BO,BO},
     {DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI},
     {EE,EE,EE,MO,WA,WA,WA,WA,WA,WA,WA,EE,EE,EE,EE,MO},
     {DI,DI,DI,DI,WA,BO,BO,BO,BO,BO,WA,DI,DI,DI,DI,DI},
     {DI,BO,DI,DI,WA,DM,DM,DM,DM,DM,WA,DI,DI,DI,DI,BO},
     {DI,DI,DI,DI,WA,DI,DI,DI,DI,DI,WA,DI,DI,DI,DI,DI},
     {DM,EE,DI,DI,WA,WA,DI,DI,WA,WA,WA,DI,DI,DI,DI,EE},
     {DM,EE,DI,DI,DM,DM,DI,DI,DM,DM,DM,DI,DI,DI,DI,EE}
   },
#endif
#if GAME_CONFIG_NOF_LEVEL>15
  {
     {PL,DI,DI,DI,WA,BO,DI,BO,BO,DI,DI,DI,DI,DI,BO,BO},
     {DI,DI,BO,BO,WA,DI,DI,DM,DM,DI,DI,DI,DI,DI,DM,DM},
     {DI,DI,DM,DM,WA,MO,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI},
     {DI,DI,DM,DM,WA,EE,DI,DI,WA,WA,WA,WA,WA,WA,DI,DI},
     {DI,DI,DM,DM,WA,DI,DI,DI,WA,DI,DI,DI,BO,BO,BO,DI},
     {DI,DI,DI,DI,WA,DM,DM,DI,WA,DI,DI,DI,DM,DM,DM,DI},
     {DI,DI,BO,BO,WA,DM,DM,DI,WA,DI,DI,DI,BO,BO,BO,DI},
     {DI,DI,DI,DI,DI,DI,DI,DI,WA,DI,DI,DI,DM,DM,DM,DI}
   },
#endif
#if GAME_CONFIG_NOF_LEVEL>16
   {
     {DI,DI,BO,BO,BO,DI,BO,BO,BO,DI,BO,BO,BO,DI,BO,BO},
     {PL,DI,DM,DM,DM,DI,DM,DM,DM,DI,DM,DM,DM,DI,DM,DM},
     {DI,DI,DI,DI,BO,BO,BO,DI,BO,BO,BO,DI,BO,BO,BO,DI},
     {DI,DI,BO,DI,DM,DM,DM,DI,DM,DM,DM,DI,DM,DM,DM,DI},
     {BO,DI,BO,DM,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI},
     {BO,DI,BO,DM,DI,DI,BO,DI,BO,DI,BO,DI,BO,DI,BO,DI},
     {DI,DI,DM,DM,DI,DI,BO,DI,BO,DI,BO,DI,BO,DI,BO,DI},
     {DI,DI,DM,DI,DI,DI,DM,DM,DM,DI,DM,DM,DM,DI,DM,DM}
   },
#endif
#if GAME_CONFIG_NOF_LEVEL>17
   {
     {DI,DI,DI,DI,BO,WA,BO,BO,BO,DI,DI,BO,BO,WA,DI,DI},
     {DI,PL,DI,DI,DM,WA,DM,DM,DM,DI,DI,DM,DM,WA,DI,DI},
     {DI,BO,BO,DI,DM,WA,DI,DI,BO,DI,DI,DM,DM,WA,DI,BO},
     {DI,DM,DM,DI,DM,WA,DI,DI,DM,WA,DI,DM,DM,WA,DI,DI},
     {DI,DM,DM,DI,DM,WA,DI,DI,DM,WA,DI,DI,DI,WA,BO,DI},
     {DI,DI,DI,DI,DM,WA,DI,DI,DM,WA,DI,DI,DI,WA,DM,DI},
     {DI,DI,DI,DI,DM,DI,DI,DI,DI,WA,DI,DI,DI,WA,DM,DI},
     {DI,DI,DI,DI,DI,DI,DI,DI,DI,WA,DM,DI,DI,DI,DI,DI}
   },
#endif
#if GAME_CONFIG_NOF_LEVEL>18
   {
     {BO,BO,DI,DI,DI,WA,DI,DI,DI,DI,BO,BO,BO,BO,DM,BO},
     {DM,DM,BO,BO,BO,WA,DI,DI,DI,DM,DM,DM,DM,DM,DM,BO},
     {DM,DM,DM,DM,DM,WA,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI},
     {DM,DM,BO,BO,BO,WA,WA,WA,WA,WA,WA,WA,WA,BO,DI,DI},
     {DI,DI,DM,DM,DM,DI,DI,BO,BO,BO,BO,BO,BO,BO,DI,DI},
     {BO,DI,DM,DM,DM,DI,DI,DM,DM,DM,DM,DM,DM,DM,DI,DI},
     {BO,DI,DM,DM,DM,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI},
     {DI,PL,DI,DI,DI,DI,DI,BO,BO,BO,BO,DI,DI,DI,DI,DI}
   },
#endif
#if GAME_CONFIG_NOF_LEVEL>19
   {
     {BO,BO,BO,WA,DI,DI,BO,BO,BO,BO,BO,DM,DM,DM,DM,BO},
     {DI,DI,DM,WA,DI,DI,DI,DI,DI,DM,DM,DI,DI,DI,DI,BO},
     {DI,PL,DM,WA,BO,BO,BO,BO,DI,DI,DI,BO,DI,DI,DI,DM},
     {BO,DI,DM,WA,DI,DM,DM,DM,DM,DI,DI,BO,BO,BO,BO,DM},
     {DM,DI,DI,WA,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI,DI},
     {DM,DM,BO,WA,DI,DI,DI,DI,DI,DI,DI,DM,DM,DM,DM,DI},
     {BO,DI,DM,DI,DI,DI,DM,DM,DM,DI,BO,BO,BO,DI,DI,DI},
     {DI,DI,DI,DI,DI,DI,DM,DM,DM,DI,DI,DI,DI,DI,DI,DI}
   },
#endif
};

static uint8_t Boulder_Tiles[BOULDER_TILE_NOF_Y][BOULDER_TILE_NOF_X];

void Boulder_SetTileAtTilePos(int8_t boulderTileX, int8_t boulderTileY, Boulder_Tile_e tile) {
  Boulder_Tiles[boulderTileY][boulderTileX] = tile;
}

Boulder_Tile_e Boulder_GetTileFromTilePos(int8_t boulderTileX, int8_t boulderTileY) {
  if (boulderTileX<0) {
    return Boulder_Tile_None;
  } else if (boulderTileX>=BOULDER_TILE_NOF_X) {
    return Boulder_Tile_None;
  }
  if (boulderTileY<0) {
    return Boulder_Tile_None;
  } else if (boulderTileY>=BOULDER_TILE_NOF_Y) {
    return Boulder_Tile_None;
  }
  return Boulder_Tiles[boulderTileY][boulderTileX];
}

Boulder_Tile_e Boulder_GetTileFromDisplayPos(int x, int y) {
  int xTile, yTile;

  if (x<0) {
    return Boulder_Tile_None;
  } else if (x>=BOULDER_DISPLAY_CONFIG_WITH) {
    return Boulder_Tile_None;
  }
  if (y<0) {
    return Boulder_Tile_None;
  } else if (y>=BOULDER_DISPLAY_CONFIG_HEIGHT) {
    return Boulder_Tile_None;
  }
  xTile = Boulder_DisplayPosToTilePosX(x);
  yTile = Boulder_DisplayPosToTilePosY(y);
  return Boulder_Tiles[yTile][xTile];
}

int8_t Boulder_GetTileToDisplayXPos(int8_t boulderTileX) {
  int x;

  if (boulderTileX<0) {
    x = 0;
  } else if (boulderTileX>=BOULDER_TILE_NOF_X) {
    x = BOULDER_TILE_NOF_X-1;
  } else{
    x = boulderTileX;
  }
  return x*BOULDER_TILE_PIXEL_SIZE;
}

int8_t Boulder_GetTileToDisplayYPos(int8_t boulderTileY) {
  int y;

  if (boulderTileY<0) {
    y = 0;
  } else if (boulderTileY>=BOULDER_TILE_NOF_Y) {
    y = BOULDER_TILE_NOF_Y-1;
  } else {
    y = boulderTileY;
  }
  return y*BOULDER_TILE_PIXEL_SIZE;
}

int Boulder_DisplayPosToTilePosX(int posX) {
  return posX/BOULDER_TILE_PIXEL_SIZE;
}

int Boulder_DisplayPosToTilePosY(int posY) {
  return posY/BOULDER_TILE_PIXEL_SIZE;
}

static Sprite_t *GetSpriteInTile(int xTile, int yTile) {
  int x0, y0;

  x0 = Boulder_GetTileToDisplayXPos(xTile);
  y0 = Boulder_GetTileToDisplayYPos(yTile);
  return Sprite_GetSpriteInPixelAreaIterated(
      Game_IteratorGetSpriteLists, /* iterator through all sprites */
      x0, y0,
      x0+BOULDER_TILE_PIXEL_SIZE-1, y0+BOULDER_TILE_PIXEL_SIZE-1);
}

static bool isMoveableBoulder(Sprite_t *boulder, int xTile, int yTile, int dx, int dy) {
  Sprite_t *sprite;

  /* xTile and yTile are the current boulder coordinate. Check if we can move it */
  if (Sprite_IsMoving(boulder)) { /* boulder already moving, e.g. falling down? */
    return false; /* cannot move a boulder which is moving */
  }
  if (dx>0 && dy==0) { /* horizontal push to the right: only if there is space to the left */
    if (xTile>=BOULDER_TILE_NOF_X-1) { /* check border situation */
      return false; /* boulder is on the right display border: cannot move it out of the display */
    }
    if (Boulder_GetTileFromTilePos(xTile+1, yTile)!=Boulder_Tile_Empty) {
      return false; /* no empty space on the right side, cannot move into it */
    }
    sprite = GetSpriteInTile(xTile+1, yTile); /* get possible sprite in location where we move the boulder */
    if (sprite==NULL) {
      return true; /* no sprite on the right side: can move into it */
    }
    return sprite==boulder; /* if there is a sprite, can only move into space if we are already (partially) in it */
  } else if (dx<0 && dy==0) { /* horizontal push to the left */
    if (xTile==0) {
      return false; /* boulder is already on the left display border: cannot move outside of the display */
    }
    if (Boulder_GetTileFromTilePos(xTile-1, yTile)!=Boulder_Tile_Empty) {
      return false; /* no empty space on the left side, cannot move into it */
    }
    sprite = GetSpriteInTile(xTile-1, yTile);
    if (sprite==NULL) {
      return true; /* no sprite on the right side: can move into it */
    }
    return sprite==boulder; /* if there is a sprite, can only move into space if we are already (partially) in it */
  }
  return false; /* default, cannot be moved */
}

Sprite_t *Boulder_GetSpriteInTileFromList(int xTile, int yTile, Sprite_t *list) {
  int x, y;

  x = Boulder_GetTileToDisplayXPos(xTile);
  y = Boulder_GetTileToDisplayYPos(yTile);
  return Sprite_GetSpriteInPixelAreaFromList(list, x, y, x+BOULDER_TILE_PIXEL_SIZE-1, y+BOULDER_TILE_PIXEL_SIZE-1);
}

Sprite_t *Boulder_GetSpriteInTileFromListIterated(int xTile, int yTile, Sprite_IteratorGetSpriteListFunction iteratorFct) {
  int cntr = 0;
   Sprite_t **sprites;
   Sprite_t *sprite;

   do {
     sprites = iteratorFct(&cntr);
     if (sprites!=NULL) {
       sprite = Boulder_GetSpriteInTileFromList(xTile, yTile, *sprites);
       if (sprite!=NULL) { /* found one */
         return sprite;
       }
     }
   } while(cntr!=0);
   return NULL;
}

void Boulder_OnPlayerMove(Sprite_t *sprite, int *deltaX, int *deltaY) {
  /* decides if the player can move. Player moves always in steps of a tile */
  int dx = *deltaX;
  int dy = *deltaY;
  Sprite_t *boulderSprite;
  bool canMove;
  int xPlayerTile, yPlayerTile;
  int xTargetTile, yTargetTile;
  Boulder_Tile_e targetTileKind;

  xPlayerTile = Boulder_DisplayPosToTilePosX(sprite->posX);
  yPlayerTile = Boulder_DisplayPosToTilePosY(sprite->posY);
  xTargetTile = xPlayerTile;
  yTargetTile = yPlayerTile;
  if (dx>0) {
    xTargetTile++;
  } else if (dx<0) {
    xTargetTile--;
  }
  if (dy>0) {
    yTargetTile++;
  } else if (dy<0) {
    yTargetTile--;
  }
  canMove = false;
  targetTileKind = Boulder_GetTileFromTilePos(xTargetTile, yTargetTile);
  switch(targetTileKind) {
    case Boulder_Tile_Empty: /* but there can be a sprite behind it */
      boulderSprite = Boulder_GetSpriteInTileFromList(xTargetTile, yTargetTile, Game_GetBoulderList());
      if (boulderSprite!=NULL) {
        if (isMoveableBoulder(boulderSprite, xTargetTile, yTargetTile, dx, dy)) { /* can we move the boulder? */
          if (dx<0) {
            dx = -BOULDER_TILE_PIXEL_SIZE; /* move boulder by a full tile */
          } else {
            dx = BOULDER_TILE_PIXEL_SIZE;
          }
          Sprite_MoveLimitedDisplay(boulderSprite, dx, 0); /* move the boulder */
          canMove = true; /* moving player too */
        }
      } else { /* no boulder: move into it */
        canMove = true;
      }
      break;

    case Boulder_Tile_Dirt:
      Boulder_SetTileAtTilePos(xTargetTile, yTargetTile, Boulder_Tile_Empty);
      canMove = true; /* can move, touched and changed to empty tile */
      break;

    case Boulder_Tile_None: /* outside display */
    case Boulder_Tile_Wall:
      canMove = false; /* cannot move into wall */
      break;

    /* the ones below should not exist any more, as represented by sprites */
    case Boulder_Tile_Player:
    case Boulder_Tile_Boulder:
    case Boulder_Tile_Diamond:
    case Boulder_Tile_Monster:
    default:
      canMove = true;
      break;
  } /* switch */
  if (!canMove) {
    *deltaX = *deltaY = 0;
    return;
  }
  /* set the actual move vector for the player */
  if (dx!=0) {
    if (dx<0) {
      *deltaX = -BOULDER_TILE_PIXEL_SIZE;
    } else {
      *deltaX = BOULDER_TILE_PIXEL_SIZE;
    }
  }
  if (dy!=0) {
    if (dy<0) {
      *deltaY = -BOULDER_TILE_PIXEL_SIZE;
    } else {
      *deltaY = BOULDER_TILE_PIXEL_SIZE;
    }
  }
}

void Boulder_CreateLevel(uint8_t level) {
  if (level>GAME_CONFIG_NOF_LEVEL-1) {
    level = GAME_CONFIG_NOF_LEVEL-1;
  }
  memcpy(Boulder_Tiles, Boulder_levels[level], sizeof(Boulder_Tiles));
  /* create sprites */
  for(int x=0; x<BOULDER_TILE_NOF_X; x++) {
    for(int y=0; y<BOULDER_TILE_NOF_Y; y++) {
      Boulder_Tile_e tile;

      tile = Boulder_Tiles[y][x];
      Game_CreateBoulderTileSprite(tile, Boulder_GetTileToDisplayXPos(x), Boulder_GetTileToDisplayYPos(y));
      switch(tile) {
        case Boulder_Tile_Boulder:
        case Boulder_Tile_Player:
        case Boulder_Tile_Diamond:
        case Boulder_Tile_Monster:
          Boulder_Tiles[y][x] = Boulder_Tile_Empty; /* draw as sprite, replace tile with empty space */
          break;
        default:
          break;
      } /* switch */
    }
  }
}

void Boulder_PaintTiles(void) {
  Boulder_Tile_e tile;
  int xPos, yPos;
  PIMAGE bitmap = NULL;

  for(int x=0; x<BOULDER_TILE_NOF_X; x++) {
    for(int y=0; y<BOULDER_TILE_NOF_Y; y++) {
      tile = Boulder_GetTileFromTilePos(x, y);
      xPos = Boulder_GetTileToDisplayXPos(x);
      yPos = Boulder_GetTileToDisplayYPos(y);
      bitmap = NULL;
      switch(tile) {
        case Boulder_Tile_Empty:
          /* draw nothing. Screen had previously cleaned */
          break;
        case Boulder_Tile_Dirt:
          bitmap = BoulderImages_GetDirt();
          break;

        case Boulder_Tile_Player:  /* handled as sprite */
        case Boulder_Tile_Diamond: /* handled as sprite */
        case Boulder_Tile_Boulder: /* handled as sprite */
        case Boulder_Tile_Monster: /* handled as sprite */
          break;

        case Boulder_Tile_Wall:
          bitmap = BoulderImages_GetWall();
          break;

        default:
          bitmap = NULL;
          break;
      } /* switch */
      if (bitmap!=NULL) {
        McuGDisplaySSD1306_DrawMonoBitmap(xPos, yPos, bitmap, McuGDisplaySSD1306_COLOR_WHITE, McuGDisplaySSD1306_COLOR_BLACK);
      }
    }
  }
}

void Boulder_Deinit(void) {
}

void Boulder_Init(void) {
}
