// packing_puzzle.cpp

#include "game.h"
#include "packing_puzzle.h"

const olc::vf2d PackingPuzzle::shape = {100, 150};

PackingPuzzle::PackingPuzzle(const PackingPuzzleAssets& packingPuzzleAssets) {
    assets = packingPuzzleAssets;

    fTickerTime = 0.0f;
    complete = false;
    numOfWords = 3;

    // Is vector best for this? Idk prob too heavy.
    // TODO : think of something w a variable size that can hold multiple coords
    for (int cellIdx = 0; cellIdx < numOfWords * 8; cellIdx++) {
        gridCellCoords.push_back({
            framePos.x,
            framePos.y 
            + ((assets.memberFrameDotted->Size().y-2) * (8*(cellIdx / 8))) 
            + ((assets.memberFrameDotted->Size().y-2) * (cellIdx % 8))
        });
    }
    

    charMember.position = gridCellCoords.at(0);
    charMember.numOfBytes = 1;
    charMember.color = GREEN;
    charMember.shape = {
        assets.memberBlue->Size().x,
        assets.memberBlue->Size().y * charMember.numOfBytes
    };
    structMembers.push_back(charMember);
    memberMap[{
        charMember.position.x, 
        charMember.position.y
    }] = charMember;

    charPtrMember.position = gridCellCoords.at(8);
    charPtrMember.numOfBytes = 8;
    charPtrMember.color = GREY;
    charPtrMember.shape = {
        assets.memberBlue->Size().x,
        assets.memberBlue->Size().y * charPtrMember.numOfBytes
    };
    structMembers.push_back(charPtrMember);
    memberMap[{
        charPtrMember.position.x, 
        charPtrMember.position.y
    }] = charPtrMember;

    shortMember.position = gridCellCoords.at(16);
    shortMember.numOfBytes = 2;
    shortMember.color = PINK;
    shortMember.shape = {
        assets.memberBlue->Size().x,
        assets.memberBlue->Size().y * shortMember.numOfBytes
    };
    structMembers.push_back(shortMember);
    memberMap[{
        shortMember.position.x, 
        shortMember.position.y
    }] = shortMember;

    intMember.position = gridCellCoords.at(18);
    intMember.numOfBytes = 4;
    intMember.color = RED;
    intMember.shape = {
        assets.memberBlue->Size().x,
        assets.memberBlue->Size().y * intMember.numOfBytes // TODO : Adjust for stacked
    };
    structMembers.push_back(intMember);
    memberMap[{
        intMember.position.x, 
        intMember.position.y
    }] = intMember;
};

void PackingPuzzle::Draw(olc::Draw& draw, float fElapsedTime) {

    // TODO : can this be in a parent class ?
    if (complete) {
        draw.StringProp({40, 45}, "Puzzle Complete!", olc::Colour::VERY_DARK_GREEN, {1.5, 1.5});
        return;
    }

    auto batch = draw.CreateImageBatch(*assets.memberFrameDotted);
    for (olc::vf2d gridCellCoord : gridCellCoords) {
        draw.Image(batch, *assets.memberFrameDotted, gridCellCoord);
    }
    draw.Batch(batch);

    // Draw struct members
    for (StructMember member: structMembers) {
        
        // Draw all bytes in a member
        olc::Image* memberBarStacked;
        olc::Image* memberBarBottom;
        switch (member.color) {
            case BLUE : 
                memberBarStacked = member.held ? assets.memberBlueStacked : assets.memberBlue;
                memberBarBottom = member.held ? assets.memberBlue : assets.memberBlueWithShadow;
            break;
            case GREEN : 
                memberBarStacked = member.held ? assets.memberGreenStacked : assets.memberGreen;
                memberBarBottom = member.held ? assets.memberGreen : assets.memberGreenWithShadow;
            break;
            case GREY : 
                memberBarStacked = member.held ? assets.memberGreyStacked : assets.memberGrey;
                memberBarBottom = member.held ? assets.memberGrey : assets.memberGreyWithShadow;
            break;
            case PINK : 
                memberBarStacked = member.held ? assets.memberPinkStacked : assets.memberPink;
                memberBarBottom = member.held ? assets.memberPink : assets.memberPinkWithShadow;
            break;
            case RED : 
                memberBarStacked = member.held ? assets.memberRedStacked : assets.memberRed;
                memberBarBottom = member.held ? assets.memberRed : assets.memberRedWithShadow;
        }

        // If not being held, collapse
        // TODO: adjust spacing between eachother so it's hugged by frame
        auto batch = draw.CreateImageBatch(*memberBarStacked);
        for (int i = 0; i < member.numOfBytes-1; i++) {
            draw.Image(
                batch, 
                *memberBarStacked, 
                {
                    static_cast<float>(member.position.x), 
                    static_cast<float>(member.position.y + ((memberBarStacked->Size().y-2) * i))
                }
            );
        }
        draw.Batch(batch);
        
        draw.Image(
            *memberBarBottom, 
            {
                static_cast<float>(member.position.x), 
                static_cast<float>(member.position.y + ((memberBarStacked->Size().y-2) * (member.numOfBytes-1)))
            }
        );
        
    }
};

bool PackingPuzzle::memberContainsPoint(StructMember& member, olc::vf2d point) {
    return member.position.x <= point.x
        && point.x <= member.position.x + member.shape.x
        && member.position.y <= point.y
        && point.y <= member.position.y + member.shape.y;
}

bool PackingPuzzle::memberNearFrame(StructMember& member, olc::vf2d framePos) {
    return member.position.x <= framePos.x + PROXIMITY_THRESHOLD_PX_X
        && member.position.x >= framePos.x - PROXIMITY_THRESHOLD_PX_X
        && member.position.y <= framePos.y + PROXIMITY_THRESHOLD_PX_Y
        && member.position.y >= framePos.y - PROXIMITY_THRESHOLD_PX_Y;
}

// TODO: is shifting vector elements expensive? idk
void PackingPuzzle::bringMemberToFront(int index) {
    StructMember member = structMembers[index];
    structMembers.erase(structMembers.begin()+index);
    structMembers.push_back(member);
}

void PackingPuzzle::Update(float fElapsedTime, const PuzzleInput& input) {
    checkComplete();

    // Held member should already be in back. Check if held and update
    if (structMembers.back().held && input.leftClickHeld) {
        // TODO : these namings are kinda confusing. need to fix
        StructMember& activeMember = structMembers.back();

        // Don't let moving member to be found in the map. Set back when released
        memberMap.erase({activeMember.position.x, activeMember.position.y});

        activeMember.position = input.mousePosition - activeMember.heldOffsetPosition;
    }

    // Held member is released
    if (structMembers.back().held && input.leftClickReleased) {
        structMembers.back().held = false;
        // Check if member should snap to a frame
        for (olc::vf2d currFramePos : gridCellCoords){
            if (memberNearFrame(structMembers.back(), currFramePos)) {
                structMembers.back().position = currFramePos;
                break;
            }
        }
        memberMap[{structMembers.back().position.x, structMembers.back().position.y}] = structMembers.back();
    }

    // Update in reverse order.
    for (int i = structMembers.size()-1; i >= 0; i--) {
        // if being grabbed, update position and exit
        if (memberContainsPoint(structMembers[i], input.mousePosition) && input.leftClickPressed) {
            bringMemberToFront(i);
            StructMember& activeMember = structMembers.back();
            activeMember.heldOffsetPosition = input.mousePosition - activeMember.position;
            activeMember.held = true;

            break;
        }
    }

};

bool PackingPuzzle::isComplete() { 
    return complete;
};

// this is O(n^2) or whatever so it could be done more efficiently
// probably with a hashmap that points to member from coords but 
// do i really want to update the hashmap everytime the member is moved
// well i guess only when it is let go... fine ill do it u convinced me
void PackingPuzzle::checkComplete() {
    int numMembersSeen = 0;
    int cellIdx = 0;
    while (cellIdx < gridCellCoords.size()) {
        // girl idk anymore im tired
        olc::vf2d gridCoords = gridCellCoords.at(cellIdx);
        // TODO : im sorry im lazy at this moment. i need to make olc vf2d hashable somehow
        if (memberMap.contains({gridCoords.x, gridCoords.y})) {
            numMembersSeen ++;
            cellIdx += memberMap[{gridCoords.x, gridCoords.y}].numOfBytes;
        } else {
            complete = (numMembersSeen == structMembers.size());
            break;
        }
    }
};