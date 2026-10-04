// puzzle_window.h

#ifndef PUZZLE_WINDOW_H
#define PUZZLE_WINDOW_H

#include "game_window.h"
#include "puzzle.h"

#include <memory>

class PuzzleWindow : public GameWindow {
private: 
	std::unique_ptr<Puzzle> puzzle;

public:
	PuzzleWindow(
		olc::PixelGameEngine& game,
		olc::vf2d p,
		float w,
		float h,
		const WindowAssets& assets,
		olc::vi2d contentSize,
		std::unique_ptr<Puzzle> pzl
	);

	void Draw(olc::Draw& draw, float fElapsedTime);
	void Update(float fElapsedTime, bool active);
};

#endif