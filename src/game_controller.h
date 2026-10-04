#pragma once

#include "game_state.h"
#include <memory>

class GameController {
public:
	//constructor
	GameController(olc::PixelGameEngine& game, olc::Image& titleCard);

	// starts the game
	void Start();

	bool Update(float fElapsedTime);

	// draw the currently active state
	void Draw();

	// returns the id of the current active state
	GameStateID GetCurrentState() const;

private:
	// performs the actual state change, calling OnExit on the old state and OnEnter on the new state
	void ChangeState(GameStateID newState);

private:
	//engineee
	olc::PixelGameEngine& game;
	
	// ref to the current active state
	std::unique_ptr<GameState> activeState;

	// id of the current active state
	GameStateID currentState = GameStateID::None;

	olc::Image& titleCard;

	bool quitRequested = false;
};