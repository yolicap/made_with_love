// Define OLC_PGE3_APPLICATION to include the implementation of
// the Pixel Game Engine as part of this translation unit
#define OLC_PGE3_APPLICATION

#include "game.h"
#include "puzzle.h"
#include "debug_puzzle.h"
#include "game_window.h"
#include "puzzle_window.h"
#include "game_controller.h"

#include <numbers>

GAME_PARAMETERS GameParameters;

float rand_float(float min, float max) {
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

float rand_int(int min, int max) {
	return min + rand() % (max - min + 1);
}

// class PackingPuzzle : public Puzzle {
// private:
// 	bool complete;

// public:
// 	PackingPuzzle(GameWindow* pw) {
// 		puzzleWindow = pw;
// 		complete = false;
// 	}

//     void Draw(olc::Draw& draw) override {
// 		puzzleWindow->Draw(draw);
// 	}

// 	void Update() override {
// 		checkComplete();
// 	}

// 	bool isComplete() override {
// 		return false;
// 	}

// private:
// 	void checkComplete() override {
// 		complete = false;
// 	}
// };

// class PuzzleWindow : public GameWindow {
// 	private:
// 	// TODO: when window is closed, pointers should be freed
// 	Puzzle* puzzle;
// 	// const olc::vf2d shape {325, 125};

// 	public:
// 	PuzzleWindow(        
// 		olc::vf2d p, 
//         float w, 
//         float h, 
//         olc::Image* wtc, 
//         olc::Image* wbc, 
//         olc::Image* wte, 
//         olc::Image* we, 
//         olc::Image* wbu,
// 		olc::Image* wc,
// 		Puzzle* pzl
// 	) : GameWindow(p, w, h, wtc, wbc, wte, we, wbu, wc) {
// 		puzzle = pzl;
// 	}

// 	void Draw(olc::Draw& draw, float fElapsedTime) {
// 		GameWindow::Draw(draw);
// 		// Draw window target
// 		draw.SetTarget(*windowContent);
// 		puzzle->Draw(draw, fElapsedTime);
// 		draw.SetTarget(*(GameParameters.screen));
// 	}

// };


class MadeWithLove : public olc::PixelGameEngine {

protected:
	// std::vector<Puzzle*> puzzles;
	std::vector<PuzzleWindow> puzzleWindows;

	olc::Image windowTopCornerImage;
	olc::Image windowBottomCornerImage;
	olc::Image windowTopEdgeImage;
	olc::Image windowEdgeImage;
	olc::Image windowButtonUp;

	olc::Image puzzleContent; // TODO: malloc this instead
	DebugPuzzle dbgPuzzle1;


private:
	GameController gameController;


public:

	MadeWithLove() : gameController(*this) {
		sAppName = "Made With Love";
	}

	// Called once when the game starts
	bool OnUserCreate() override
	{
		GameParameters.screen = &GetScreen();

		CreateImageFromFile(windowTopCornerImage, "./assets/Window_Corner_Top.png");
		CreateImageFromFile(windowBottomCornerImage, "./assets/Window_Corner_Bottom.png");
		CreateImageFromFile(windowTopEdgeImage, "./assets/Window_Edge_Top.png");
		CreateImageFromFile(windowEdgeImage, "./assets/Window_Edge_Side+Bottom.png");
		CreateImageFromFile(windowButtonUp, "./assets/Close_Button_Up.png");

		olc::vf2d pos{ 100.0, 100.0 };

		// TODO: debug puzzle shapes should be set in globals
		CreateImage(puzzleContent, { 325, 125 });
		dbgPuzzle1 = DebugPuzzle();

		puzzleWindows.emplace_back(PuzzleWindow(
			pos,
			325,
			125,
			&windowTopCornerImage,
			&windowBottomCornerImage,
			&windowTopEdgeImage,
			&windowEdgeImage,
			&windowButtonUp,
			&puzzleContent,
			&dbgPuzzle1
		));

		gameController.Start();

		return true;
	}


	// Called every frame
	bool OnUserUpdate(float fElapsedTime) override
	{
		GameParameters.screen = &GetScreen();
		GameParameters.leftClickPressed = mouse.GetButton(0).bPressed;
		GameParameters.leftClickHeld = mouse.GetButton(0).bHeld;
		GameParameters.leftClickReleased = mouse.GetButton(0).bReleased;

		GameParameters.mousePosition = mouse.GetPosition();

		for(int i = 0; i < puzzleWindows.size(); i++) {
            puzzleWindows[i].Update();
        }

		//Update the game controller, which will update the current state
		gameController.Update(fElapsedTime);

		if (gameController.GetCurrentState() == GameStateID::Gameplay)
		{
			draw.Clear(olc::Colour::BLACK);

			for (int i = 0; i < puzzleWindows.size(); i++) {
				puzzleWindows[i].Draw(draw, fElapsedTime);
			}
		}
		else {
			gameController.Draw();
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