// game_window.h
// #pragma once

#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

class GameWindow {
	// this is a vector btw
    olc::vf2d position; 

	float width;
	float height;

	olc::Image* windowTopCorner;
	olc::Image* windowBottomCorner;
	olc::Image* windowTopEdge;
	olc::Image* windowEdge;
	olc::Image* windowButtonUp;

    public:
    // TODO: when window is closed, THIS pointer should be freed
    olc::Image* windowContent;

    GameWindow(
        olc::vf2d p, 
        float w, 
        float h, 
        olc::Image* wtc, 
        olc::Image* wbc, 
        olc::Image* wte, 
        olc::Image* we, 
        olc::Image* wbu,
        olc::Image* wc
    );
    void Draw(olc::Draw& draw);

	private:
	void DrawEdges(olc::Draw& draw, float x1, float y1, float x2, float y2);
	void DrawCorners(olc::Draw& draw, float x1, float y1, float x2, float y2);

};

#endif