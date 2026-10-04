#pragma once

#include "game_state.h"
#include "game.h"

class GameOverState : public GameState {
public:
	GameOverState(olc::PixelGameEngine& game);

	void OnEnter() override;

	void OnExit() override;

	GameStateID Update(float fElapsedTime) override;

	void Draw() override;
};