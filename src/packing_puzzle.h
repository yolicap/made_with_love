// packing_puzzle.h

#ifndef PACKING_PUZZLE_H
#define PACKING_PUZZLE_H

#include "puzzle.h"

struct PackingPuzzleAssets {
	olc::Image* memberBlue = nullptr;
    olc::Image* memberBlueStacked = nullptr;
	olc::Image* memberBlueWithShadow = nullptr;
	olc::Image* memberGreen = nullptr;
    olc::Image* memberGreenStacked = nullptr;
	olc::Image* memberGreenWithShadow = nullptr;
	olc::Image* memberGrey = nullptr;
    olc::Image* memberGreyStacked = nullptr;
	olc::Image* memberGreyWithShadow = nullptr;
	olc::Image* memberPink = nullptr;
    olc::Image* memberPinkStacked = nullptr;
	olc::Image* memberPinkWithShadow = nullptr;
    olc::Image* memberRed = nullptr;
    olc::Image* memberRedStacked = nullptr;
	olc::Image* memberRedWithShadow = nullptr;
};

enum MemberColor {
    BLUE,
    GREEN,
    GREY,
    PINK,
    RED
};

class PackingPuzzle : public Puzzle {
private:
	static const olc::vf2d shape;
    PackingPuzzleAssets assets;

    uint8_t numOfWords;

	float fTickerTime;
	bool complete;

    struct StructMember {
        olc::vi2d position;
        olc::vi2d shape;
        uint8_t numOfBytes;
        MemberColor color;
        bool held;
        olc::vf2d heldOffsetPosition;
    } charMember, charPtrMember, shortMember, intMember;

    std::vector<StructMember> structMembers;

public:
	PackingPuzzle(const PackingPuzzleAssets& packingPuzzleAssets);
    void Draw(olc::Draw& draw, float fElapsedTime) override;
	void Update(float fElapsedTime, const PuzzleInput& input) override;
	bool isComplete() override;

private:
	void checkComplete() override;
    bool memberContainsPoint(StructMember& member, olc::vf2d point);
    void bringMemberToFront(int index);

};

#endif