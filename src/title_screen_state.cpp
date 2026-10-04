#include "title_screen_state.h"

TitleScreenState::TitleScreenState(olc::PixelGameEngine& game) : GameState(game) {}

void TitleScreenState::OnEnter() {
	// placeholder for any later code
}


void TitleScreenState::OnExit() {
	// placeholder for any later code
}

GameStateID TitleScreenState::Update(float fElapsedTime) {
	(void)fElapsedTime;

	// mouse button 0 is the left mouse button
	if (game.GetMouse().GetButton(0).bPressed && IsMouseInsideStartButton()) {
		return GameStateID::Gameplay;
	}

	return GameStateID::None;
}


void TitleScreenState::Draw() {
	olc::Draw& draw = game.GetDraw();

	// background
	draw.Clear(
		olc::Pixel(
			30,
			20,
			28
		)
	);

	// title
	const std::string title =
		"MADE WITH LOVE";

	const olc::vf2d titleScale = {
		3.0f,
		3.0f
	};

	const auto titleSize = draw.GetTextSize(title, true, titleScale);

	const float titleX = (static_cast<float>(game.ScreenSize().x) - static_cast<float>(titleSize.x)) / 2.0f;

	draw.StringProp(
		{
			titleX,
			90.0f
		},
		title,
		olc::Colour::WHITE,
		titleScale
	);

	//start button
	draw.FilledRect(startButtonPosition, startButtonSize, olc::Pixel(180, 75, 100));

	const std::string buttonText = "START";

	const olc::vf2d buttonTextScale =
	{
		2.0f,
		2.0f
	};

	const auto buttonTextSize = draw.GetTextSize(buttonText, true, buttonTextScale);

	const olc::vf2d buttonTextPosition =
	{
		startButtonPosition.x + (startButtonSize.x - static_cast<float>(buttonTextSize.x)) / 2.0f,
		startButtonPosition.y + (startButtonSize.y - static_cast<float>(buttonTextSize.y)) / 2.0f
	};

	draw.StringProp(buttonTextPosition, buttonText, olc::Colour::WHITE, buttonTextScale);
}

bool TitleScreenState::IsMouseInsideStartButton() const
{
	const olc::vf2d mousePosition = game.GetMouse().GetPosition();

	const bool insideX = mousePosition.x >= startButtonPosition.x && mousePosition.x <= startButtonPosition.x + startButtonSize.x;
	const bool insideY = mousePosition.y >= startButtonPosition.y && mousePosition.y <= startButtonPosition.y + startButtonSize.y;

	return insideX && insideY;
}