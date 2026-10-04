// Define OLC_PGE3_APPLICATION to include the implementation of
// the Pixel Game Engine as part of this translation unit
#define OLC_PGE3_APPLICATION

#include "game.h"
#include "debug_puzzle.h"
#include "packing_puzzle.h"
#include "game_controller.h"
#include "puzzle_window.h"
#include "window_manager.h"

#include <memory>
#include <numbers>

GAME_PARAMETERS GameParameters;

float rand_float(float min, float max) {
	return min + static_cast<float>(rand()) /
		static_cast<float>(RAND_MAX / (max - min));
}

float rand_int(int min, int max) {
	return min + rand() % (max - min + 1);
}

class MadeWithLove : public olc::PixelGameEngine {
protected:
	olc::Image windowTopCornerImage;
	olc::Image windowBottomCornerImage;
	olc::Image windowTopEdgeImage;
	olc::Image windowEdgeImage;
	olc::Image windowButtonUp;
	olc::Image windowButtonDown;

	olc::Image cursor;
	olc::Image cursorGrab;
	olc::Image cursorGrabbing;

	olc::Image monitorBorder;
	olc::Image titleCard;
	// getting state from the controller is hard
	float fGameplayTimeRemaining;


	olc::Image memberBlue;
	olc::Image memberBlueStacked;
	olc::Image memberBlueWithShadow;
	olc::Image memberGreen;
	olc::Image memberGreenStacked;
	olc::Image memberGreenWithShadow;
	olc::Image memberGrey;
	olc::Image memberGreyStacked;
	olc::Image memberGreyWithShadow;
	olc::Image memberPink;
	olc::Image memberPinkStacked;
	olc::Image memberPinkWithShadow;
    olc::Image memberRed;
	olc::Image memberRedStacked;
	olc::Image memberRedWithShadow;

private:
	WindowManager windowManager;
	GameController gameController;

public:
	MadeWithLove() : gameController(*this, titleCard) {
		sAppName = "Made With Love";
	}

	// Called once when the game starts
	bool OnUserCreate() override {
		GameParameters.screen = &GetScreen();

		CreateImageFromFile(windowTopCornerImage, "./assets/Window_Corner_Top.png");
		CreateImageFromFile(windowBottomCornerImage, "./assets/Window_Corner_Bottom.png");
		CreateImageFromFile(windowTopEdgeImage, "./assets/Window_Edge_Top.png");
		CreateImageFromFile(windowEdgeImage, "./assets/Window_Edge_Side+Bottom.png");
		CreateImageFromFile(windowButtonUp, "./assets/Close_Button_Up.png");
		CreateImageFromFile(windowButtonDown, "./assets/Close_Button_Down.png");
		CreateImageFromFile(cursor, "./assets/Cursor/Cursor_Default.png");
		CreateImageFromFile(cursorGrab, "./assets/Cursor/Cursor_Grab.png");
		CreateImageFromFile(cursorGrabbing, "./assets/Cursor/Cursor_Grabbing.png");
		CreateImageFromFile(monitorBorder, "./assets/Monitor_Border.png");
		CreateImageFromFile(titleCard, "./assets/Title_card.png");
		CreateImageFromFile(memberBlue, "./assets/Struct/Struct_Blue.png");
		CreateImageFromFile(memberBlueStacked, "./assets/Struct/Struct_Blue_Stacked.png");
		CreateImageFromFile(memberBlueWithShadow, "./assets/Struct/Struct_Blue_With_Shadow.png");
		CreateImageFromFile(memberGreen, "./assets/Struct/Struct_Green.png");
		CreateImageFromFile(memberGreenStacked, "./assets/Struct/Struct_Green_Stacked.png");
		CreateImageFromFile(memberGreenWithShadow, "./assets/Struct/Struct_Green_With_Shadow.png");
		CreateImageFromFile(memberGrey, "./assets/Struct/Struct_Grey.png");
		CreateImageFromFile(memberGreyStacked, "./assets/Struct/Struct_Grey_Stacked.png");
		CreateImageFromFile(memberGreyWithShadow, "./assets/Struct/Struct_Grey_With_Shadow.png");
		CreateImageFromFile(memberPink, "./assets/Struct/Struct_Pink.png");
		CreateImageFromFile(memberPinkStacked, "./assets/Struct/Struct_Pink_Stacked.png");
		CreateImageFromFile(memberPinkWithShadow, "./assets/Struct/Struct_Pink_With_Shadow.png");
		CreateImageFromFile(memberRed, "./assets/Struct/Struct_Red.png");
		CreateImageFromFile(memberRedStacked, "./assets/Struct/Struct_Red_Stacked.png");
		CreateImageFromFile(memberRedWithShadow, "./assets/Struct/Struct_Red_With_Shadow.png");

		WindowAssets windowAssets = {
			&windowTopCornerImage,
			&windowBottomCornerImage,
			&windowTopEdgeImage,
			&windowEdgeImage,
			&windowButtonUp,
			&windowButtonDown
		};

		PackingPuzzleAssets packingPuzzleAssets = {
			&memberBlue,
			&memberBlueStacked,
			&memberBlueWithShadow,
			&memberGreen,
			&memberGreenStacked,
			&memberGreenWithShadow,
			&memberGrey,
			&memberGreyStacked,
			&memberGreyWithShadow,
			&memberPink,
			&memberPinkStacked,
			&memberPinkWithShadow,
    		&memberRed,
			&memberRedStacked,
			&memberRedWithShadow,
		};

		// First debug puzzle window
		windowManager.AddWindow(
			std::make_unique<PuzzleWindow>(
				*this,
				olc::vf2d{ 100.0f, 90.0f },
				220.0f,
				120.0f,
				windowAssets,
				olc::vi2d{ 325, 125 },
				std::make_unique<DebugPuzzle>()
			)
		);

		// Second debug puzzle window.
		// Uses a separate DebugPuzzle instance so the puzzles
		// do not share state.
		windowManager.AddWindow(
			std::make_unique<PuzzleWindow>(
				*this,
				olc::vf2d{ 260.0f, 140.0f },
				220.0f,
				120.0f,
				windowAssets,
				olc::vi2d{ 325, 125 },
				std::make_unique<DebugPuzzle>()
			)
		);

		// Packing puzzle 
		windowManager.AddWindow(
			std::make_unique<PuzzleWindow>(
				*this,
				olc::vf2d{ 260.0f, 140.0f },
				220.0f,
				120.0f,
				windowAssets,
				olc::vi2d{ 325, 125 },
				std::make_unique<PackingPuzzle>(packingPuzzleAssets)
			)
		);

		draw.SetTarget(GetScreen());

		gameController.Start();
		// ShowMouseCursor(false);

		return true;
	}

	void DrawTimer() {
		char clockStr[6];
		int minsRemaining = (fGameplayTimeRemaining + 1) / 60;
		int secsRemaining = (fGameplayTimeRemaining - minsRemaining * 60) + 1;
		secsRemaining = secsRemaining == 60 ? 0 : secsRemaining;
		snprintf(clockStr, sizeof(clockStr), "%.2d:%.2d", minsRemaining, secsRemaining);
		olc::vf2d timerPos = {518, 342};
		const olc::vf2d normalScale = { 1.5f, 1.5f };
		draw.FilledRect({515,338},{57,20}, olc::Pixel(100,100,100), olc::Colour::DARK_GREY);
		draw.StringProp(timerPos, clockStr, GAME_TEXT_COLOR, normalScale);
		draw.SetTarget(GetScreen());
	}


	// Called every frame
	bool OnUserUpdate(float fElapsedTime) override {
		GameParameters.screen = &GetScreen();
		GameParameters.leftClickPressed = mouse.GetButton(0).bPressed;
		GameParameters.leftClickHeld = mouse.GetButton(0).bHeld;
		GameParameters.leftClickReleased = mouse.GetButton(0).bReleased;

		GameParameters.mousePosition = mouse.GetPosition();

		GameStateID prevState = gameController.GetCurrentState();

		//Update the game controller, which will update the current state
		if (!gameController.Update(fElapsedTime)) {
			return false;
		}
		GameStateID currentState = gameController.GetCurrentState();
		draw.SetTarget(GetScreen());

		if (currentState == GameStateID::Gameplay) {

			if (prevState != GameStateID::Gameplay) {
				windowManager.OpenAll();
				fGameplayTimeRemaining = GAME_TIME_LIMIT;
			}

			draw.Clear(GAME_BACKGROUND_COLOR);
			windowManager.Update(fElapsedTime);
			windowManager.Draw(draw, fElapsedTime);
			fGameplayTimeRemaining -= fElapsedTime;
		}
		else {
			draw.SetTarget(GetScreen());
			gameController.Draw();
		}

		// TODO : this needs to be in its own class with isHovering methods and all that
		// just for demo.. it stays here
		// if(GameParameters.leftClickHeld)
		// 	draw.Image(cursorGrabbing, GameParameters.mousePosition);
		// else
		// 	draw.Image(cursorGrab, GameParameters.mousePosition);

		// always make sure the monitor border is rendered to the screen, after everything else has been drawn
		draw.SetTarget(GetScreen());

		olc::vf2d borderScale = {
			static_cast<float>(ScreenSize().x) / static_cast<float>(monitorBorder.Size().x),
			static_cast<float>(ScreenSize().y) / static_cast<float>(monitorBorder.Size().y)
		};

		draw.Image(
			monitorBorder,
			{ 0.0f, 0.0f },
			borderScale
		);

		if (gameController.GetCurrentState() == GameStateID::Gameplay) {
			DrawTimer();
		}

		return true;
	}
};

int main() {
	MadeWithLove game;

	// Configure the game window
	PGEConfig config;

	// Wait for the display refresh before presenting each frame
	config.bVSync = true;

	// Each logical pixel becomes 2x2 actual window pixels
	config.vPixelSize = { 2, 2 };

	// Game screen size in logical pixels
	config.vScreenSize = { 640, 360 };

	if (game.Construct(config)) {
		game.Start();
	}

	return 0;
}