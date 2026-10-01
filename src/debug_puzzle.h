// debug_puzzle.h

#ifndef DEBUG_PUZZLE_H
#define DEBUG_PUZZLE_H

#include "puzzle.h"

class DebugPuzzle : public Puzzle {
private:
	static const olc::vf2d shape; 
	std::string sBackgroundRandomText;

	// olc::ImageRegion region;
	float fTickerTime;
	bool complete;

public:
	DebugPuzzle();
    void Draw(olc::Draw& draw, float fElapsedTime) override;
	void Update() override;
	bool isComplete() override;

private:
	void checkComplete() override;
};

#endif