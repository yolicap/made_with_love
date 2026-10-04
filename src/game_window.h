// game_window.h

#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include "olcPixelGameEngine3.h"

struct WindowAssets {
	olc::Image* topCorner = nullptr;
	olc::Image* bottomCorner = nullptr;
	olc::Image* topEdge = nullptr;
	olc::Image* edge = nullptr;
	olc::Image* closeButtonUp = nullptr;
	olc::Image* closeButtonDown = nullptr;
};

class GameWindow {
public:
	GameWindow(
		olc::PixelGameEngine& game,
		olc::vf2d position,
		float width,
		float height,
		const WindowAssets& assets,
		olc::vi2d contentSize
	);

	virtual ~GameWindow() = default;

	void Draw(olc::Draw& draw);
	void Update(bool active);
	void Open();

	bool IsOpen() const;
	bool ContainsPoint(const olc::vf2d& point) const;

protected:
	olc::Image& GetWindowContent();
	olc::vf2d GetContentOrigin() const;

protected:
	olc::vf2d position;

private:
	float width;
	float height;

	WindowAssets assets;
	olc::Image windowContent;

	bool held;
	olc::vf2d heldOffsetPosition;

    bool open;
    bool mouseOverCloseButton;

private:
	void DrawEdges(
		olc::Draw& draw,
		float x1,
		float y1,
		float x2,
		float y2
	);

	void DrawCorners(
		olc::Draw& draw,
		float x1,
		float y1,
		float x2,
		float y2
	);

	bool onHandle(const olc::vf2d& point) const;

	bool inBounds(
		float x,
		float y,
		float xMin,
		float yMin,
		float xMax,
		float yMax
	) const;
};

#endif