// packing_puzzle.h

#ifndef PACKING_PUZZLE_H
#define PACKING_PUZZLE_H

#include "puzzle.h"

class PackingPuzzle : public Puzzle {
private:
	static const olc::vf2d shape;

	float fTickerTime;
	bool complete;

public:
	PackingPuzzle();
    void Draw(olc::Draw& draw, float fElapsedTime) override;
	void Update(float fElapsedTime) override;
	bool isComplete() override;

private:
	void checkComplete() override;
	bool onMember(olc::vf2d point, olc::vf2d memberHitbox);
};

#endif