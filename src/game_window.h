// game_window.h
// #pragma once

#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

class GameWindow {

	float width;
	float height;

	olc::Image* windowTopCorner;
	olc::Image* windowBottomCorner;
	olc::Image* windowTopEdge;
	olc::Image* windowEdge;
	olc::Image* windowButtonUp;
	olc::Image* windowButtonDown;

    bool held;
    olc::vf2d heldOffsetPosition;

    bool open;
    bool holdCloseButton;

    protected:
    olc::vf2d position; 

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
        olc::Image* wbd,
        olc::Image* wc
    );
    void Draw(olc::Draw& draw);
    void Update();
    void Open();

	private:
	void DrawEdges(olc::Draw& draw, float x1, float y1, float x2, float y2);
	void DrawCorners(olc::Draw& draw, float x1, float y1, float x2, float y2);
    bool onHandle(olc::vf2d point);
    bool inBounds(float x, float y, float xMin, float yMin, float xMax, float yMax);

};

#endif