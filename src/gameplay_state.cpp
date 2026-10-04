#include <olcPixelGameEngine3.h>

#include "debug_puzzle.h"
#include "puzzle_window.h"
#include "gameplay_state.h"
#include "game.h"

GameplayState::GameplayState(olc::PixelGameEngine& game, WindowAssets inWindowAssets) : GameState(game) {
	fTimeRemaining = GAME_TIME_LIMIT;
	windowAssets = inWindowAssets;

		// First debug puzzle window
		windowManager.AddWindow(
			std::make_unique<PuzzleWindow>(
				game,
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
				game,
				olc::vf2d{ 260.0f, 140.0f },
				220.0f,
				120.0f,
				windowAssets,
				olc::vi2d{ 325, 125 },
				std::make_unique<DebugPuzzle>()
			)
		);
}

void GameplayState::OnEnter() {
	// gameplay initialization can go here later
	windowManager.OpenAll();
}


void GameplayState::OnExit() {
	// gameplay cleanup can go here later
	fTimeRemaining = GAME_TIME_LIMIT;
}

GameStateID GameplayState::Update(float fElapsedTime) {
	fTimeRemaining -= fElapsedTime;
	windowManager.Update(fElapsedTime);

	// TEMPORARY DEBUG CONTROL!!!
	// Press G to test Game Over state
	// Will remove once gameover conditions are implemented
	if (game.GetKeyboard().GetKey(olc::Key::G).bPressed ||
        (fTimeRemaining <= 0)) {
		return GameStateID::GameOver;
	}

	return GameStateID::None;
}

void GameplayState::DrawOverlay() {
	olc::Draw& draw = game.GetDraw();
	char clockStr[6];
	int minsRemaining = (fTimeRemaining + 1) / 60;
	int secsRemaining = (fTimeRemaining - minsRemaining * 60) + 1;
	secsRemaining = secsRemaining == 60 ? 0 : secsRemaining;
	snprintf(clockStr, sizeof(clockStr), "%.2d:%.2d", minsRemaining, secsRemaining);
	olc::vf2d timerPos = {518, 342};
	const olc::vf2d normalScale = { 1.5f, 1.5f };
	draw.FilledRect({515,338},{57,20}, olc::Pixel(100,100,100), olc::Colour::DARK_GREY);
	draw.StringProp(timerPos, clockStr, GAME_TEXT_COLOR, normalScale);
}

void GameplayState::Draw() {
	olc::Draw& draw = game.GetDraw();

	draw.Clear(GAME_BACKGROUND_COLOR);
	windowManager.Draw(draw);
}