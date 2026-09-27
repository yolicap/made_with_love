// Define OLC_PGE3_APPLICATION to include the implementation of
// the Pixel Game Engine as part of this translation unit
#define OLC_PGE3_APPLICATION
#include "olcPixelGameEngine3.h"

class MadeWithLove : public olc::PixelGameEngine {

public:

	// Called once when the game starts
	bool OnUserCreate() override
	{
		return true;
	}


	// Called every frame
	bool OnUserUpdate(float fElapsedTime) override
	{
		return true;
	}

};


int main()
{
	MadeWithLove game;

	// Configure the game window
	PGEConfig config;

	// Wait for the display refresh before presenting each frame
	config.bVSync = true;

	// Each logical pixel becomes 2x2 actual window pixels
	config.vPixelSize = { 2, 2 };

	// Game screen size in logical pixels
	config.vScreenSize = { 640, 360 };


	if (game.Construct(config))
	{
		game.Start();
	}

	return 0;
}