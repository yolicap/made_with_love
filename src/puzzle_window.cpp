// puzzle_window.cpp

#include "game.h"
#include "puzzle_window.h"

PuzzleWindow::PuzzleWindow(        
    olc::vf2d p, 
    float w, 
    float h, 
    olc::Image* wtc, 
    olc::Image* wbc, 
    olc::Image* wte, 
    olc::Image* we, 
    olc::Image* wbu,
    olc::Image* wc,
    Puzzle* pzl
) : GameWindow(p, w, h, wtc, wbc, wte, we, wbu, wc) {
    puzzle = pzl;
}

void PuzzleWindow::Draw(olc::Draw& draw, float fElapsedTime) {
    GameWindow::Draw(draw);
    // Draw window target
    draw.SetTarget(*windowContent);
    puzzle->Draw(draw, fElapsedTime);
    draw.SetTarget(*(GameParameters.screen));
}