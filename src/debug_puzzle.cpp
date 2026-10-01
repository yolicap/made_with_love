// debug_puzzle.cpp

#include "game.h"
#include "debug_puzzle.h"

#include <string>

const olc::vf2d DebugPuzzle::shape = {200, 100}; 

// olc::ImageRegion region;

DebugPuzzle::DebugPuzzle() {
    // CreateImage(region, shape);
    fTickerTime = 0.0f;
    complete = false;
}

void DebugPuzzle::Draw(olc::Draw& draw, float fElapsedTime) {
    // draw.SetTarget(region);

    // Determine how many characters fit on the screen
    // olc::vf2d vSizeOfChar = draw.GetTextSize("A");
    // olc::vf2d nVisibleChars = shape.x / vSizeOfChar;

    draw.StringProp(
        { 10, 10 }, 
        std::format(
            "[{:#04x}]:{:#08x}--------------------[stack]",
            0x0000, 
            0x00000000
        ), 
        olc::Pixel(0x3f, 0xa0, 0xa4) 
    );

    for (float i = 0; i < 8; i++) {
        draw.StringProp(
            { 10, 20 + (10 * i) }, 
            std::format(
                "{:#08x} : {:#02x} {:#02x} {:#02x} {:#02x} {}", 
                0x00000000, 
                0x00, 
                0x00, 
                0x00, 
                0x00, 
                ". . . . . . . . . . . . . . . ."
            ),  
            olc::Pixel(0x81, 0xaf, 0xb5) // #81afb5
        );
    }

}

void DebugPuzzle::Update() {
    checkComplete();
}

bool DebugPuzzle::isComplete() {
    return false;
}

void DebugPuzzle::checkComplete() {
    complete = false;
}