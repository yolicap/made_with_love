#pragma once

#include "olcPixelGameEngine3.h"

inline const olc::Pixel GAME_BACKGROUND_COLOR = olc::Pixel(0xe3, 0xf5, 0xf1);
inline const olc::Pixel GAME_TEXT_COLOR = olc::Pixel(0x9f, 0x3a, 0x52);

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