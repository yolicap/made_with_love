#include "olcPixelGameEngine3.h"

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