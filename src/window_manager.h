#pragma once

#include "puzzle_window.h"

#include <memory>
#include <vector>

class WindowManager {
public:
	void AddWindow(std::unique_ptr<PuzzleWindow> window);

	void Update(float fElapsedTime);
	void Draw(olc::Draw& draw, float fElapsedTime);

	void OpenAll();

private:
	void BringToFront(size_t index);
	void SelectTopOpenWindow();

private:
	// Vector order is also window draw order.
	// The last window is the window currently on top.
	std::vector<std::unique_ptr<PuzzleWindow>> windows;

	PuzzleWindow* activeWindow = nullptr;
};