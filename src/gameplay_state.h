#pragma once

#include "game_state.h"
#include "game.h"
#include "game_window.h"
#include "window_manager.h"

#define TIME_LIMIT_SECS (5 * 60)

class GameplayState : public GameState {
public:
	float fTimeRemaining;

	GameplayState(olc::PixelGameEngine& game, WindowAssets inWindowAssets);

	void OnEnter() override;

	void OnExit() override;

	GameStateID Update(float fElapsedTime) override;

	void Draw() override;

	void DrawOverlay() override;
private:
	WindowManager windowManager;
	WindowAssets windowAssets;

};