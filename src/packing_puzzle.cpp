// packing_puzzle.cpp

#include "game.h"
#include "packing_puzzle.h"

const olc::vf2d PackingPuzzle::shape = {100, 200};

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
            + ((assets.memberFrameDotted->Size().y) * (8*(cellIdx / 8))) 
            + (assets.memberFrameDotted->Size().y * (cellIdx % 8))
        });
    }
    

    charMember.position = {
        static_cast<int>(framePos.x), 
        // static_cast<int>(framePos.y + (*( % 8)) + (( / 8)))
        10
    };
    charMember.numOfBytes = 1;
    charMember.color = GREEN;
    charMember.shape = {
        assets.memberBlue->Size().x,
        assets.memberBlue->Size().y * charMember.numOfBytes
    };
    structMembers.push_back(charMember);

    charPtrMember.position = {
        static_cast<int>(framePos.x), 
        20
    };
    charPtrMember.numOfBytes = 8;
    charPtrMember.color = GREY;
    charPtrMember.shape = {
        assets.memberBlue->Size().x,
        assets.memberBlue->Size().y * charPtrMember.numOfBytes
    };
    structMembers.push_back(charPtrMember);

    shortMember.position = {
        static_cast<int>(framePos.x), 
        50
    };
    shortMember.numOfBytes = 2;
    shortMember.color = PINK;
    shortMember.shape = {
        assets.memberBlue->Size().x,
        assets.memberBlue->Size().y * shortMember.numOfBytes
    };
    structMembers.push_back(shortMember);

    intMember.position = {
        static_cast<int>(framePos.x), 
        80
    };
    intMember.numOfBytes = 4;
    intMember.color = RED;
    intMember.shape = {
        assets.memberBlue->Size().x,
        assets.memberBlue->Size().y * intMember.numOfBytes // TODO : Adjust for stacked
    };
    structMembers.push_back(intMember);
};

void PackingPuzzle::Draw(olc::Draw& draw, float fElapsedTime) {

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

        // TODO: If not being held, collapse
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

    // Held member should already be in back. Check if held and update
    if (structMembers.back().held && input.leftClickHeld) {
        // TODO : these namings are kinda confusing. need to fix
        StructMember& activeMember = structMembers.back();
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

void PackingPuzzle::checkComplete() {
    complete = false;
};