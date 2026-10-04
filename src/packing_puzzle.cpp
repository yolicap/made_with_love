// packing_puzzle.cpp

#include "game.h"
#include "packing_puzzle.h"

	static const olc::vf2d shape = {200, 100};

	PackingPuzzle::PackingPuzzle(const PackingPuzzleAssets& packingPuzzleAssets) {
        assets = packingPuzzleAssets;

        fTickerTime = 0.0f;
        complete = false;

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
            assets.memberBlue->Size().y * intMember.numOfBytes
        };
        structMembers.push_back(intMember);
    };

    void PackingPuzzle::Draw(olc::Draw& draw, float fElapsedTime) {

        // Draw struct frame

        // Draw struct members
        for (StructMember& member: structMembers) {

            // Draw all bytes in a member
            olc::Image* memberBarWithShadow;
            olc::Image* memberBar;
            switch (member.color) {
                case BLUE : 
                    memberBarWithShadow = assets.memberBlueWithShadow;
                    memberBar = assets.memberBlue;
                break;
                case GREEN : 
                    memberBarWithShadow = assets.memberGreenWithShadow;
                    memberBar = assets.memberGreen;
                break;
                case GREY : 
                    memberBarWithShadow = assets.memberGreyWithShadow;
                    memberBar = assets.memberGrey;
                break;
                case PINK : 
                    memberBarWithShadow = assets.memberPinkWithShadow;
                    memberBar = assets.memberPink;
                break;
                case RED : 
                    memberBarWithShadow = assets.memberRedWithShadow;
                    memberBar = assets.memberRed;
            }

            auto batch = draw.CreateImageBatch(*memberBarWithShadow);
            for (int i = 0; i < member.numOfBytes; i++) {
                draw.Image(
                    batch, 
                    *memberBarWithShadow, 
                    {
                        static_cast<float>(member.position.x), 
                        static_cast<float>(member.position.y + ((memberBar->Size().y - 1) * i))
                    }
                );
            }
            draw.Batch(batch);
        }
    };

    bool PackingPuzzle::memberContainsPoint(StructMember& member, olc::vf2d point) {
        return member.position.x <= point.x
            && point.x <= member.position.x + member.shape.x
            && member.position.y <= point.y
            && point.y <= member.position.y + member.shape.y;
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
            // TODO: remove shadow
            StructMember& activeMember = structMembers.back();
            activeMember.position = input.mousePosition - activeMember.heldOffsetPosition;
        }

        if (structMembers.back().held && input.leftClickReleased) {
            structMembers.back().held = false;
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