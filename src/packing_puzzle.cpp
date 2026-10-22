// packing_puzzle.cpp

#include "game.h"
#include "packing_puzzle.h"

const olc::vf2d PackingPuzzle::shape = {100, 180};

PackingPuzzle::PackingPuzzle(const PackingPuzzleAssets& packingPuzzleAssets) {
    assets = packingPuzzleAssets;

    fTickerTime = 0.0f;
    complete = false;
    numOfWords = 3;

    charMember.position = {10, 10};
    charMember.numOfBytes = 1;
    charMember.color = GREEN;
    charMember.shape = {
        assets.memberBlue->Size().x,
        assets.memberBlue->Size().y * charMember.numOfBytes
    };
    structMembers.push_back(charMember);

    charPtrMember.position = {20, 20};
    charPtrMember.numOfBytes = 8;
    charPtrMember.color = GREY;
    charPtrMember.shape = {
        assets.memberBlue->Size().x,
        assets.memberBlue->Size().y * charPtrMember.numOfBytes
    };
    structMembers.push_back(charPtrMember);

    shortMember.position = {30, 30};
    shortMember.numOfBytes = 2;
    shortMember.color = PINK;
    shortMember.shape = {
        assets.memberBlue->Size().x,
        assets.memberBlue->Size().y * shortMember.numOfBytes
    };
    structMembers.push_back(shortMember);

    intMember.position = {40, 40};
    intMember.numOfBytes = 4;
    intMember.color = RED;
    intMember.shape = {
        assets.memberBlue->Size().x,
        assets.memberBlue->Size().y * intMember.numOfBytes // TODO : Adjust for stacked
    };
    structMembers.push_back(intMember);
};

void PackingPuzzle::Draw(olc::Draw& draw, float fElapsedTime) {

    // Draw struct frame
    for (int word = 0; word < numOfWords; word++) {
        auto batch = draw.CreateImageBatch(*assets.memberFrameDotted);
        for (int byte = 0; byte < 8; byte++) {
            draw.Image(
                batch, 
                *assets.memberFrameDotted, 
                {
                    static_cast<float>(framePos.x), 
                    static_cast<float>(
                        framePos.y 
                        + (((assets.memberFrameDotted->Size().y-2) * 7) * word)
                        + (( assets.memberFrameDotted->Size().y) * byte) 
                    )
                }
            );
        }
        draw.Batch(batch);
    }

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
        olc::vf2d currFramePos = {framePos.x, framePos.y};
        for (int frame = 0; frame < (8 * 3); frame++) {
            // Increment y axis
            if ( memberNearFrame(structMembers.back(), currFramePos) ){
                structMembers.back().position = currFramePos;
                break;
            }
            currFramePos.y += (assets.memberFrameDotted->Size().y-2); 
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