// Define OLC_PGE3_APPLICATION to include the implementation of
// the Pixel Game Engine as part of this translation unit
#define OLC_PGE3_APPLICATION

#include "game.h"
#include "game_window.h"

#include <numbers>

float rand_float(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(max - min)));
}

float rand_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

class MadeWithLove : public olc::PixelGameEngine {

protected:
    std::vector<GameWindow> gameWindows;
	olc::Image windowTopCornerImage;
	olc::Image windowBottomCornerImage;
	olc::Image windowTopEdgeImage;
	olc::Image windowEdgeImage;
	olc::Image windowButtonUp;

public:

	// Called once when the game starts
	bool OnUserCreate() override
	{

		CreateImageFromFile(windowTopCornerImage, "./assets/Window_Corner_Top.png");
		CreateImageFromFile(windowBottomCornerImage, "./assets/Window_Corner_Bottom.png");
		CreateImageFromFile(windowTopEdgeImage, "./assets/Window_Edge_Top.png");
		CreateImageFromFile(windowEdgeImage, "./assets/Window_Edge_Side+Bottom.png");
		CreateImageFromFile(windowButtonUp, "./assets/Close_Button_Up.png");

		olc::vf2d pos {100.0, 100.0};
        gameWindows.emplace_back(GameWindow(
			pos, 
			200, 
			100, 
			&windowTopCornerImage, 
			&windowBottomCornerImage,
			&windowTopEdgeImage,
			&windowEdgeImage,
			&windowButtonUp
		));

		return true;
	}


	// Called every frame
	bool OnUserUpdate(float fElapsedTime) override
	{

		// Clear screen to a background color
		draw.Clear(olc::Colour::BLACK);

        // Draw windows
        for(int i = 0; i < gameWindows.size(); i++) {
            gameWindows[i].Draw(draw);
        }

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