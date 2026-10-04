#pragma once

#include "game_state.h"

class TitleScreenState : public GameState {
public:
	TitleScreenState(olc::PixelGameEngine& game);

	void OnEnter() override;

	void OnExit() override;

	GameStateID Update(float fElapsedTime) override;

	void Draw() override;

private:
	bool IsMouseInsideStartButton() const;

private:
	olc::vf2d startButtonPosition =
	{
		230.0f,
		210.0f
	};

	olc::vf2d startButtonSize =
	{
		180.0f,
		50.0f
	};
};