// puzzle_window.h

#ifndef PUZZLE_WINDOW_H
#define PUZZLE_WINDOW_H

#include "game_window.h"
#include "puzzle.h"

class PuzzleWindow : public GameWindow {

	// TODO: when window is closed, pointers should be freed
	Puzzle* puzzle;

	public:
	PuzzleWindow(        
		olc::vf2d p, 
        float w, 
        float h, 
        olc::Image* wtc, 
        olc::Image* wbc, 
        olc::Image* wte, 
        olc::Image* we, 
        olc::Image* wbu,
        olc::Image* wbd,
		olc::Image* wc,
		Puzzle* pzl
	);

	void Draw(olc::Draw& draw, float fElapsedTime);
	void Update(float fElapsedTime);

};

#endif