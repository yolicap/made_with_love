// puzzle_window.cpp

#include "game.h"
#include "puzzle_window.h"

#include <utility>

PuzzleWindow::PuzzleWindow(
	olc::PixelGameEngine& game,
	olc::vf2d p,
	float w,
	float h,
	WindowAssets& assets,
	olc::vi2d contentSize,
	std::unique_ptr<Puzzle> pzl
) : GameWindow(
	game,
	p,
	w,
	h,
	assets,
	contentSize
),
puzzle(std::move(pzl)) {
}

void PuzzleWindow::Draw(olc::Draw& draw) {
	if (!IsOpen()) return;

	// Draw window target
	draw.SetTarget(GetWindowContent());

	olc::Pixel backgroundColor = olc::Pixel(0xe3, 0xf5, 0xf1);

	draw.Clear(backgroundColor);

	puzzle->Draw(draw);

	draw.SetTarget(*(GameParameters.screen));

	GameWindow::Draw(draw);
}

void PuzzleWindow::Update(float fElapsedTime, bool active) {
	GameWindow::Update(active);
	if (!IsOpen()) return;

	PuzzleInput input;

	input.active = active;

	// this is ghetto af im sorry
	// Convert the global mouse position into the puzzle's local window position.
	// dw about it, I see the vision -bs
	input.mousePosition = GameParameters.mousePosition - GetContentOrigin();
	input.leftClickPressed = active && GameParameters.leftClickPressed;
	input.leftClickHeld = active && GameParameters.leftClickHeld;
	input.leftClickReleased = active && GameParameters.leftClickReleased;
	puzzle->Update(fElapsedTime, input);
}