// packing_puzzle.cpp

#include "game.h"
#include "packing_puzzle.h"

	static const olc::vf2d shape = {200, 100};

	PackingPuzzle::PackingPuzzle() {
        fTickerTime = 0.0f;
        complete = false;
    };

    void PackingPuzzle::Draw(olc::Draw& draw, float fElapsedTime) {

    };

	void PackingPuzzle::Update(float fElapsedTime) {

    };

	bool PackingPuzzle::isComplete() {
        return complete;
    };

	void PackingPuzzle::checkComplete() {
        complete = false;
    };

	bool PackingPuzzle::onMember(olc::vf2d point, olc::vf2d memberHitbox) {
        return false;
    }