#pragma once

#include "olcPixelGameEngine3.h"

// Enum for different game states
enum class GameStateID {
	None,
	TitleScreen,
	Gameplay,
	GameOver
};

class GameState {
public:
	// constructor
	GameState(olc::PixelGameEngine& game) : game(game) {}

	// virtual destructor
	virtual ~GameState() = default;

	// called once when switching into this state
	virtual void OnEnter() {}

	// called once when switching out of the state
	virtual void OnExit() {}


	// called every frame
	// return GameStateID::None to remain in the current state
	// return another GameStateID to request a state change
	virtual GameStateID Update(float fElapsedTime) = 0;

	// draws the state
	virtual void Draw() = 0;


protected:
	// ref to the game engine
	olc::PixelGameEngine& game;
};