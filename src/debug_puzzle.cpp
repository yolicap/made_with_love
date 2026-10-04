// debug_puzzle.cpp

#include "game.h"
#include "debug_puzzle.h"

#include <algorithm>
#include <format>
#include <iterator>
#include <string>

const olc::vf2d DebugPuzzle::shape = {200, 100};
olc::vf2d DebugPuzzle::bugMatrixOffset = { 0.0f, 20.0f };

// olc::ImageRegion region;

DebugPuzzle::DebugPuzzle() {
	// CreateImage(region, shape);
	fTickerTime = 0.0f;
	complete = false;

	// TODO : random location
	bugs[{0,0}] = 1;
	bugs[{1,1}] = 1;
	bugs[{2,2}] = 2;
	bugs[{3,3}] = 3;

}

void DebugPuzzle::Draw(olc::Draw& draw) {

    if (complete) {
        draw.StringProp({40, 45}, "Puzzle Complete!", olc::Colour::VERY_DARK_GREEN, {1.5, 1.5});
        return;
    }

	// Determine how many characters fit on the screen
	olc::vf2d vSizeOfChar = draw.GetTextSize("x");
	// olc::vf2d nVisibleChars = shape.x / vSizeOfChar;

	bugCharacterWidth = vSizeOfChar.x;

	bugMatrixOffset = {
		static_cast<float>(draw.GetTextSize("0x00 : 0x00 0x00").x),
		20.0
	};

	draw.StringProp(
		{ 10, 10 },
		std::format(
			"[{:#04x}]:{:#04x}-----------[stack]",
			0x00,
			0x00
		),
		olc::Pixel(0x3f, 0xa0, 0xa4)
	);

	for (float i = 0; i < 8; i++) {
		draw.StringProp(
			{ 10, 20 + (10 * i) },
			std::format(
				"{:#04x} : {:#04x} {:#04x}",
				0x00000000,
				0x00,
				0x00
			),
			olc::Pixel(0x81, 0xaf, 0xb5) // #81afb5
		);
	}

	// wow guys im so smart.. i used memset() :nerd-emoji:
	// memset(bugMatrix, '.', (8 * 8) * sizeof(char) );
	// nvm i ended up not using this but it wouldve been cool

	// TODO : maybe draw batch? that would be good. yes so good
	// TODO : there's like a billion better ways to do this but we do like this for now
	std::string rowString;
	for (int row = 0; row < 8; row++) {
		rowString = "";
		for (int col = 0; col < 8; col++) {
			// draw bug or dot
			if (!bugs.contains({row,col}))
				rowString += '.';
				// rowString += bugMatrix[col + row];
			else {
				// TODO: this could also just be a map but it needs to be working so we do this during clean up
				olc::Pixel bugColor;
				switch(bugs[{row,col}]) {
				// #632240
				case 3 : bugColor = olc::Pixel(0x63, 0x22, 0x40);
				break;
				// #bc5960
				case 2 : bugColor = olc::Pixel(0xbc, 0x59, 0x60);
				break;
				// #e2b570
				case 1 : default: bugColor = olc::Pixel(0xe2, 0xb5, 0x70);
				break;
				}

				// TODO : change color based on number of squishes required
				rowString += "  ";

				draw.StringProp(
					{ bugMatrixOffset.x + (bugCharacterWidth * col) , bugMatrixOffset.y + (10.0f * row) },
					"x",
					bugColor
				);
				// std::cout << bugMatrixOffset.x + (vSizeOfChar.x * col) << " " << bugMatrixOffset.y + (10 * row) << std::endl;

			}

			rowString += "  ";
		}

		draw.StringProp(
			{
				bugMatrixOffset.x,
				bugMatrixOffset.y + (10.0f * row)
			},
			rowString,
			olc::Pixel(0x81, 0xaf, 0xb5) // #81afb5
		);
	}
}

std::vector<std::pair<int, int>> DebugPuzzle::getPossibleBugMovement(std::pair<int, int> currPos) {
	std::vector<std::pair<int, int>> possiblePos;

	// east
	if (!bugs.contains({currPos.first - 1, currPos.second}) && currPos.first - 1 >= 0)
		possiblePos.push_back({currPos.first - 1, currPos.second});
	// west
	if (!bugs.contains({currPos.first + 1, currPos.second}) && currPos.first + 1 < 8)
		possiblePos.push_back({currPos.first + 1, currPos.second});
	// north
	if (!bugs.contains({currPos.first, currPos.second-1}) && currPos.second - 1 >= 0)
		possiblePos.push_back({currPos.first, currPos.second-1});
	// south
	if (!bugs.contains({currPos.first, currPos.second+1}) && currPos.second + 1 < 8)
		possiblePos.push_back({currPos.first, currPos.second+1});
	return possiblePos;
}

// TODO : i guess we could have a generalized global function like inRange() or smt
bool DebugPuzzle::onBug(const olc::vf2d& point, BugPosition bugPosition) const {
	// TODO: all of this needs to be redone at some point
	// position is now relative to the puzzle window itself rather than fixed coords -bs
	olc::vf2d bugPositionOnWindow = {
		bugMatrixOffset.x + (bugCharacterWidth * bugPosition.second),
		bugMatrixOffset.y + (10.0f * bugPosition.first)
	};
	return point.x > bugPositionOnWindow.x 
		&& point.x < bugPositionOnWindow.x + 10.0
		&& point.y > bugPositionOnWindow.y 
		&& point.y < bugPositionOnWindow.y + 10.0;
}

void DebugPuzzle::Update(float fElapsedTime, const PuzzleInput& input) {
	checkComplete();

	// Check if bug is boop'd
	if (input.active && input.leftClickPressed) {
		std::vector<std::pair<BugPosition, int>> bugVector;
		copy(bugs.begin(), bugs.end(), back_inserter(bugVector));

		for (auto& bug : bugVector) {
			if (onBug(input.mousePosition, bug.first)) {
				bugs[bug.first]--;

				// TODO: obv will result in some bugs not spawining. solution is select random available space in grind. implement in next verion
				std::vector<std::pair<int, int>> movement = getPossibleBugMovement(bug.first);
				// Bugs multiplying!
				for (int i = 0; i < bugs[bug.first]+1 && !movement.empty(); i++) {
					bugs[movement.back()] = bugs[bug.first];
					movement.pop_back();
				}

				// Original bug killed
				bugs.erase(bug.first);
			}
		}
	}

	// Update bug position
	// u might be asking.. y df did i use a map ?
	// i just didnt want to iterate through a matrix
	// instead of O(m * m) .. this will execute in O(num of bugs)
	fTickerTime += fElapsedTime;
	if (fTickerTime > 1.0f) {
		fTickerTime = 0.0f;
		std::vector<std::pair<BugPosition, int>> bugVector;
		copy(bugs.begin(), bugs.end(), back_inserter(bugVector));

		for (auto& bug : bugVector) {
			// if ure reading this.. meow :3
			// if ur reading this... bark >:)
			std::vector<std::pair<int, int>> movement = getPossibleBugMovement(bug.first);

			if (!movement.empty()) {
				int randomIndex = rand_int(0, static_cast<int>(movement.size()) - 1);
				bugs[movement[randomIndex]] = bug.second;
				bugs.erase(bug.first);
			}
		}
	}
}

bool DebugPuzzle::isComplete() {
	return complete;
}

void DebugPuzzle::checkComplete() {
	complete = bugs.empty();
}