#include "title_screen_state.h"

TitleScreenState::TitleScreenState(olc::PixelGameEngine& game, std::function<void()> onQuit) 
	: GameState(game), onQuit(onQuit) {
}

void TitleScreenState::OnEnter() {
	selectedOption = 0;
	showingCredits = false;
}

void TitleScreenState::OnExit() {
}

GameStateID TitleScreenState::Update(float fElapsedTime) {
	(void)fElapsedTime;

	if (showingCredits) {
		if (game.GetMouse().GetButton(0).bPressed) {
			showingCredits = false;
		}

		return GameStateID::None;
	}

	bool mouseOverOption = false;

	for (int i = 0; i < static_cast<int>(menuOptions.size()); i++) {
		if (IsMouseOverOption(i)) {
			selectedOption = i;
			mouseOverOption = true;
			break;
		}
	}

	if (mouseOverOption && game.GetMouse().GetButton(0).bPressed) {
		switch (static_cast<MenuOption>(selectedOption)) {
		case MenuOption::Start:
			return GameStateID::Gameplay;

		case MenuOption::Credits:
			showingCredits = true;
			break;

		case MenuOption::Quit:
			if (onQuit) {
				onQuit();
			}
			break;
		}
	}

	return GameStateID::None;
}

void TitleScreenState::Draw() {
	olc::Draw& draw = game.GetDraw();
	draw.Clear(olc::Colour::BLACK);

	if (showingCredits) {
		DrawCredits();
	} else {
		DrawMainMenu();
	}
}

void TitleScreenState::DrawMainMenu() {
	olc::Draw& draw = game.GetDraw();

	const std::string title = "MADE WITH LOVE";
	const olc::vf2d titleScale = { 3.0f, 3.0f };

	auto titleSize = draw.GetTextSize(title, true, titleScale);

	float titleX = (static_cast<float>(game.ScreenSize().x) - static_cast<float>(titleSize.x)) / 2.0f;

	draw.StringProp(
		{ titleX, 80.0f },
		title,
		olc::Colour::WHITE,
		titleScale
	);

	for (int i = 0; i < static_cast<int>(menuOptions.size()); i++) {
		bool mouseHovering = IsMouseOverOption(i);

		auto textSize = draw.GetTextSize(menuOptions[i], true, normalScale);

		float textX = (static_cast<float>(game.ScreenSize().x) - static_cast<float>(textSize.x)) / 2.0f;

		float textY = menuStartY + static_cast<float>(i) * menuSpacing;

		draw.StringProp(
			{ textX, textY },
			menuOptions[i],
			olc::Colour::WHITE,
			normalScale
		);

		if (mouseHovering) {
			DrawHeart({
				textX - 32.0f,
				textY + 1.0f
				});
		}
	}
}

void TitleScreenState::DrawHeart(const olc::vf2d& position) {
	olc::Draw& draw = game.GetDraw();

	const float pixelSize = 3.0f;

	const int heart[6][7] = {
		{ 0, 1, 1, 0, 1, 1, 0 },
		{ 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1 },
		{ 0, 1, 1, 1, 1, 1, 0 },
		{ 0, 0, 1, 1, 1, 0, 0 },
		{ 0, 0, 0, 1, 0, 0, 0 }
	};

	const olc::Pixel heartColor = olc::Pixel(145, 58, 82);

	for (int y = 0; y < 6; y++) {
		for (int x = 0; x < 7; x++) {
			if (heart[y][x] == 1) {
				draw.FilledRect(
					{
						position.x + x * pixelSize,
						position.y + y * pixelSize
					},
					{ pixelSize, pixelSize },
					heartColor
				);
			}
		}
	}
}

void TitleScreenState::DrawCredits() {
	olc::Draw& draw = game.GetDraw();

	const std::string title = "CREDITS";
	const olc::vf2d titleScale = { 3.0f, 3.0f };

	auto titleSize = draw.GetTextSize(title, true, titleScale);

	float titleX = (static_cast<float>(game.ScreenSize().x) - static_cast<float>(titleSize.x)) / 2.0f;

	draw.StringProp(
		{ titleX, 60.0f },
		title,
		olc::Colour::WHITE,
		titleScale
	);

	float nameY = 135.0f;

	for (const std::string& name : credits) {
		const olc::vf2d nameScale = { 1.5f, 1.5f };

		auto nameSize = draw.GetTextSize(name, true, nameScale);

		float nameX = (static_cast<float>(game.ScreenSize().x) - static_cast<float>(nameSize.x)) / 2.0f;

		draw.StringProp(
			{ nameX, nameY },
			name,
			olc::Colour::WHITE,
			nameScale
		);

		nameY += 28.0f;
	}

	const std::string returnText = "CLICK TO RETURN";

	auto returnSize = draw.GetTextSize(
		returnText,
		true,
		{ 1.0f, 1.0f }
	);

	float returnX = (static_cast<float>(game.ScreenSize().x) - static_cast<float>(returnSize.x)) / 2.0f;

	draw.StringProp(
		{ returnX, 315.0f },
		returnText,
		olc::Colour::WHITE
	);
}

bool TitleScreenState::IsMouseOverOption(int index) const {
	olc::Draw& draw = game.GetDraw();

	const std::string& text = menuOptions[index];

	auto textSize = draw.GetTextSize(text, true, normalScale);

	float textX = (static_cast<float>(game.ScreenSize().x) - static_cast<float>(textSize.x)) / 2.0f;

	float textY = menuStartY + static_cast<float>(index) * menuSpacing;

	olc::vf2d mouse = game.GetMouse().GetPosition();

	return mouse.x >= textX &&
		mouse.x <= textX + static_cast<float>(textSize.x) &&
		mouse.y >= textY &&
		mouse.y <= textY + static_cast<float>(textSize.y);
}