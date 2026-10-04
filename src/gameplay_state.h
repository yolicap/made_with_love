#pragma once

#include "game_state.h"

class GameplayState : public GameState {
public:
	GameplayState(olc::PixelGameEngine& game);

	void OnEnter() override;

	void OnExit() override;

	GameStateID Update(float fElapsedTime) override;

	void Draw() override;
};