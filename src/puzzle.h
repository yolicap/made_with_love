// puzzle.h

#ifndef PUZZLE_H
#define PUZZLE_H

#include "olcPixelGameEngine3.h"

struct PuzzleInput {
	olc::vf2d mousePosition = { 0.0f, 0.0f };

	bool active = false;
	bool leftClickPressed = false;
	bool leftClickHeld = false;
	bool leftClickReleased = false;
};

class Puzzle {
private:
	bool complete;

public:
	virtual ~Puzzle() = default;

	virtual void Draw(olc::Draw& draw) = 0;
	virtual void Update(float fElapsedTime, const PuzzleInput& input) = 0;
	virtual bool isComplete() = 0;

private:
	virtual void checkComplete() = 0;
};

#endif