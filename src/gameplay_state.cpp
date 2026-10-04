#include "gameplay_state.h"

GameplayState::GameplayState(olc::PixelGameEngine& game) : GameState(game) {}

void GameplayState::OnEnter() {
	// gameplay initialization can go here later
}


void GameplayState::OnExit() {
	// gameplay cleanup can go here later
}

GameStateID GameplayState::Update(float fElapsedTime) {
	(void)fElapsedTime;

	// TEMPORARY DEBUG CONTROL!!!
	// Press G to test Game Over state
	// Will remove once gameover conditions are implemented
	if (game.GetKeyboard().GetKey(olc::Key::G).bPressed) {
		return GameStateID::GameOver;
	}

	return GameStateID::None;
}

void GameplayState::Draw() {
	olc::Draw& draw = game.GetDraw();

	draw.Clear(olc::Colour::BLACK);

}