#include "game_over_state.h"

GameOverState::GameOverState(olc::PixelGameEngine& game) : GameState(game) {}

void GameOverState::OnEnter() {
	// placeholder for now
}

void GameOverState::OnExit() {
	// placeholder for now
}

GameStateID GameOverState::Update(float fElapsedTime)
{
	(void)fElapsedTime;

	// TEMPORARY DEBUG CONTROL!!!
	// Click ENTER to return to the title screen
	if (game.GetKeyboard().GetKey(olc::Key::ENTER).bPressed) {
		return GameStateID::TitleScreen;
	}

	return GameStateID::None;
}

void GameOverState::Draw()
{
	olc::Draw& draw = game.GetDraw();

	draw.Clear(olc::Colour::BLACK);

	const std::string gameOverText = "GAME OVER";

	const olc::vf2d titleScale =
	{
		3.0f,
		3.0f
	};

	const auto titleSize = draw.GetTextSize(gameOverText, true, titleScale);

	const float titleX = (static_cast<float>(game.ScreenSize().x) - static_cast<float>(titleSize.x)) / 2.0f;

	draw.StringProp(
		{
			titleX,
			110.0f
		},
		gameOverText,
		olc::Colour::WHITE,
		titleScale
	);

	const std::string returnText = "PRESS ENTER TO RETURN";

	const olc::vf2d returnScale =
	{
		1.0f,
		1.0f
	};

	const auto returnSize = draw.GetTextSize(returnText, true, returnScale);

	const float returnX = (static_cast<float>(game.ScreenSize().x) - static_cast<float>(returnSize.x)) / 2.0f;

	draw.StringProp(
		{
			returnX,
			200.0f
		},
		returnText,
		olc::Colour::WHITE,
		returnScale
	);
}