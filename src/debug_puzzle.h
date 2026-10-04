// debug_puzzle.h

#ifndef DEBUG_PUZZLE_H
#define DEBUG_PUZZLE_H

#include "puzzle.h"

#include <map>
#include <string>
#include <utility>
#include <vector>

typedef std::pair<int, int> BugPosition;
typedef std::map<BugPosition, int> BugMap;

class DebugPuzzle : public Puzzle {
private:
	static const olc::vf2d shape;
	static olc::vf2d bugMatrixOffset;
	std::string sBackgroundRandomText;

	// olc::ImageRegion region;
	float fTickerTime;
	bool complete;

	// int value represents number of squishes to be destroyed
	BugMap bugs;

	char* bugMatrix;

	float bugCharacterWidth = 8.0f;

public:
	DebugPuzzle();

	void Draw(olc::Draw& draw, float fElapsedTime) override;
	void Update(float fElapsedTime, const PuzzleInput& input) override;
	bool isComplete() override;

private:
	void checkComplete() override;
	std::vector<std::pair<int, int>> getPossibleBugMovement(std::pair<int, int> currPos);
	bool onBug(const olc::vf2d& point, BugPosition bugPosition) const;
};

#endif