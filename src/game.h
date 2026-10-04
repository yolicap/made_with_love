#pragma once

#include "olcPixelGameEngine3.h"
#define GAME_TIME_LIMIT (5)

inline const olc::Pixel GAME_BACKGROUND_COLOR = olc::Pixel(0x07, 0x0c, 0x1d);
inline const olc::Pixel GAME_TEXT_COLOR = olc::Pixel(0xee, 0xe9, 0x8a);

// "global" variables go here :D

// don't ask me about this
typedef struct
{
    olc::Image* screen;
    olc::vf2d mousePosition;
    bool leftClickPressed;
    bool leftClickHeld;
    bool leftClickReleased;

} GAME_PARAMETERS;

extern GAME_PARAMETERS GameParameters;
extern float rand_int(int min, int max);