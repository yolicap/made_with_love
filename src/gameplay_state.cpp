#include "gameplay_state.h"
#include "game.h"

GameplayState::GameplayState(olc::PixelGameEngine& game) : GameState(game) {
	fTimeRemaining = GAME_TIME_LIMIT;
}

void GameplayState::OnEnter() {
	// gameplay initialization can go here later
}


void GameplayState::OnExit() {
	// gameplay cleanup can go here later
	fTimeRemaining = GAME_TIME_LIMIT;
}

GameStateID GameplayState::Update(float fElapsedTime) {
	fTimeRemaining -= fElapsedTime;

	// TEMPORARY DEBUG CONTROL!!!
	// Press G to test Game Over state
	// Will remove once gameover conditions are implemented
	if (game.GetKeyboard().GetKey(olc::Key::G).bPressed ||
        (fTimeRemaining <= 0)) {
		return GameStateID::GameOver;
	}

	return GameStateID::None;
}

void GameplayState::Draw() {
	olc::Draw& draw = game.GetDraw();

	draw.Clear(olc::Colour::BLACK);

}