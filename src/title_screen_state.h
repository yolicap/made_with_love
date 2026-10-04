#pragma once

#include "game_state.h"
#include "game.h"

#include <functional>
#include <string>
#include <vector>

class TitleScreenState : public GameState {
public:
	TitleScreenState(olc::PixelGameEngine& game, olc::Image& titleCard, std::function<void()> onQuit);

	void OnEnter() override;

	void OnExit() override;

	GameStateID Update(float fElapsedTime) override;

	void Draw() override;

private:
	enum class MenuOption {
		Start = 0,
		Credits = 1,
		Quit = 2
	};

	bool IsMouseOverOption(int index) const;

	void DrawMainMenu();

	void DrawCredits();

	void DrawHeart(const olc::vf2d& position);

private:
	olc::Image& titleCard;
	std::function<void()> onQuit;

	std::vector<std::string> menuOptions = {
		"START",
		"CREDITS",
		"QUIT"
	};

	std::vector<std::string> credits = {
		"AlfaDrottning",
		"JustBrailey",
		"Put ur User",
		"Put ur User"
	};

	int selectedOption = 0;

	bool showingCredits = false;

	const float menuStartY = 180.0f;
	const float menuSpacing = 42.0f;

	const olc::vf2d normalScale = { 2.0f, 2.0f };
	const olc::vf2d selectedScale = { 2.25f, 2.25f };
};
