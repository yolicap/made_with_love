/*
*   game_window.cpp
*
*   this is a really useful comment ! /jk
*   not cleanest code but i just wanna move to the next task for now LOLLLL
*	^ real -bs
*   author : yolicap
*/

#include "game.h"
#include "game_window.h"
#include <numbers>

GameWindow::GameWindow(olc::PixelGameEngine& game, olc::vf2d p, float w, float h, const WindowAssets& windowAssets, olc::vi2d contentSize) {
	position = p;
	width = w;
	height = h;

	assets = windowAssets;
	game.CreateImage(windowContent, contentSize);

	held = false;
	open = true;
	mouseOverCloseButton = false;
	heldOffsetPosition = { 0.0f, 0.0f };
}

void GameWindow::Draw(olc::Draw& draw) {
	const auto origin = olc::vf2d{0.0f, 0.0f};

	// hide closed window
	if (!open) return;

	float x1, y1, x2, y2;
	x1 = position.x;
	y1 = position.y;
	x2 = position.x + width;
	y2 = position.y + height;

	// Draw background
	draw.Image(
		windowContent.region(
			{ 0, 0 },
			{
				width,
				height - assets.topCorner->Size().y
			}
		),
		{
			x1 + assets.topCorner->Size().x,
			y1 + assets.topCorner->Size().y
		}
	);

	DrawEdges(draw, x1, y1, x2, y2);
	DrawCorners(draw, x1, y1, x2, y2);

	// Draw button
	draw.Image(
		mouseOverCloseButton
		? *assets.closeButtonDown
		: *assets.closeButtonUp,
		{
			x2 - assets.closeButtonUp->Size().x +
				assets.topCorner->Size().x,
			y1
		}
	);
}

void GameWindow::DrawEdges(olc::Draw& draw, float x1, float y1, float x2, float y2) {
	auto batch = draw.CreateImageBatch(*assets.edge);

	// bottom edge
	draw.Image(
		batch,
		*assets.edge,
		{x1+ assets.bottomCorner->Size().x, y2},
		{(x2-x1-assets.bottomCorner->Size().x) / assets.edge->Size().x, 1.0f}
	);

	// right edge
	draw.ImageRotated(
		// batch, // TODO: bug (?) wont let me use batch. scaling ends up being done incorrectly
		*assets.edge,
		{ x2, y2 },
		(std::numbers::pi_v<float> / 2.0f) * -1.0f,
		{ 0.0f, 0.0f},
		{ (y2-y1-assets.topCorner->Size().y)/assets.edge->Size().x, 1.0f }
	);

	// left edge
	draw.ImageRotated(
		// batch,
		*assets.edge,
		{x1, y1+ assets.topCorner->Size().y},
		(std::numbers::pi_v<float> / 2.0f),
		{ 0.0f, static_cast<float>(assets.edge->Size().y)},{(y2 - y1 - assets.topCorner->Size().y) /assets.edge->Size().x,1.0f}
	);

	draw.Batch(batch);

	// top edge
	draw.Image(
		*assets.topEdge,
		{
			x1 + assets.topCorner->Size().x,
			y1
		},
		{
			(x2 - x1 - assets.topCorner->Size().x) /
				assets.topEdge->Size().x,
			1.0f
		}
	);
}

void GameWindow::DrawCorners(olc::Draw& draw, float x1, float y1, float x2, float y2) {
	// Draw corners
	auto batch = draw.CreateImageBatch(*assets.topCorner);
	draw.Image(batch, *assets.topCorner, {x1, y1});
	draw.Image(batch, assets.topCorner->flipH(), {x2, y1});

	draw.Batch(batch);

	batch = draw.CreateImageBatch(*assets.bottomCorner);

	draw.Image(batch, *assets.bottomCorner, {x1, y2});

	draw.Image(batch,assets.bottomCorner->flipH(), {x2, y2}
	);

	draw.Batch(batch);
}

bool GameWindow::inBounds(float x, float y, float xMin, float yMin, float xMax, float yMax) const {
	return (xMin < x) && (x < xMax) && (yMin < y) && (y < yMax);
}

bool GameWindow::onHandle(const olc::vf2d& point) const {
	return inBounds(
		point.x,
		point.y,
		position.x,
		position.y,
		position.x + width,
		position.y + assets.topEdge->Size().y
	);
}

bool GameWindow::ContainsPoint(const olc::vf2d& point) const {
	if (!open) {
		return false;
	}

	return inBounds(
		point.x,
		point.y,
		position.x,
		position.y,
		position.x + width,
		position.y + height
	);
}

void GameWindow::Update(bool active) {
	// prevent interacting with a closed window
	if (!open) return;

	// only active window is allowed to respond to mouse clicks
	if (!active) {
		held = false;
		return;
	}

	mouseOverCloseButton = inBounds(
		GameParameters.mousePosition.x,
		GameParameters.mousePosition.y,
		position.x + width - assets.closeButtonUp->Size().x,
		position.y,
		position.x + width,
		position.y + assets.closeButtonUp->Size().y
	);


	// If clicked on top, set held
	if (GameParameters.leftClickPressed && onHandle(GameParameters.mousePosition)) {
		held = true;
		// Set offset position
		heldOffsetPosition = GameParameters.mousePosition - position;
	}

	// If mouse is held, continue and drag
	if (held && GameParameters.leftClickHeld) {
		// Update position
		position = GameParameters.mousePosition - heldOffsetPosition;
	}

	// Otherwise, remove held
	if (held && GameParameters.leftClickReleased) {
		held = false;
		if (mouseOverCloseButton) {
			open = false;
		}
	}

	// handle button presses
	if (GameParameters.leftClickPressed) {
		if (mouseOverCloseButton) {
			open = false;
			held = false;
		}
	}
}

void GameWindow::Open() {
	open = true;
}

bool GameWindow::IsOpen() const {
	return open;
}

olc::Image& GameWindow::GetWindowContent() {
	return windowContent;
}

olc::vf2d GameWindow::GetContentOrigin() const {
	return {
		position.x + assets.topCorner->Size().x,
		position.y + assets.topCorner->Size().y
	};
}