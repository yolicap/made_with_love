#include "game_controller.h"

#include "title_screen_state.h"
#include "gameplay_state.h"
#include "game_over_state.h"

GameController::GameController(olc::PixelGameEngine& game) : game(game) {}

void GameController::Start() {
	ChangeState(GameStateID::TitleScreen);
}

bool GameController::Update(float fElapsedTime) {
	if (!activeState) {
		return true;
	}

	GameStateID requestedState = activeState->Update(fElapsedTime);

	if (quitRequested) {
		return false;
	}

	// state returns None when it wants to stay active
	if (requestedState != GameStateID::None) {
		ChangeState(requestedState);
	}

	return true;
}

void GameController::Draw() {
	if (!activeState) return;

	activeState->Draw();
}


GameStateID GameController::GetCurrentState() const {
	return currentState;
}


void GameController::ChangeState(GameStateID newState) {
	if (activeState) {
		activeState->OnExit();
	}

	// destroy the previous state
	activeState.reset();

	// create the new state
	switch (newState) {
	case GameStateID::TitleScreen:
		activeState = std::make_unique<TitleScreenState>(
			game,
			[this]() {
				quitRequested = true;
			}
		);
		break;

	case GameStateID::Gameplay:
		activeState = std::make_unique<GameplayState>(game);
		break;

	case GameStateID::GameOver:
		activeState = std::make_unique<GameOverState>(game);
		break;

	case GameStateID::None:
		break;
	}

	// if a valid state was created, store its id and notify it that it has become active
	if (activeState) {
		currentState = newState;
		activeState->OnEnter();
	}
	else {
		currentState = GameStateID::None;
	}
}