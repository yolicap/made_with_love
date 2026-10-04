#include "window_manager.h"
#include "game.h"

#include <utility>

void WindowManager::AddWindow(std::unique_ptr<PuzzleWindow> window) {
	if (!window) {
		return;
	}

	activeWindow = window.get();

	windows.push_back(
		std::move(window)
	);
}

void WindowManager::Update(float fElapsedTime) {
	if (GameParameters.leftClickPressed) {
		int clickedWindow = -1;

		// Search backwards because the final item in the vector
		// is drawn on top of all previous windows.
		for (
			int i = static_cast<int>(windows.size()) - 1;
			i >= 0;
			i--
			) {
			if (
				windows[i]->IsOpen() &&
				windows[i]->ContainsPoint(GameParameters.mousePosition)
				) {
				clickedWindow = i;
				break;
			}
		}

		if (clickedWindow >= 0) {
			BringToFront(
				static_cast<size_t>(clickedWindow)
			);

			activeWindow =
				windows.back().get();
		}
	}

	for (auto& window : windows) {
		bool active =
			window.get() == activeWindow;

		window->Update(
			fElapsedTime,
			active
		);
	}

	if (
		activeWindow &&
		!activeWindow->IsOpen()
		) {
		SelectTopOpenWindow();
	}
}

void WindowManager::Draw(
	olc::Draw& draw
) {
	for (auto& window : windows) {
		window->Draw(
			draw
		);
	}
}

void WindowManager::OpenAll() {
	for (auto& window : windows) {
		window->Open();
	}

	SelectTopOpenWindow();
}

void WindowManager::BringToFront(size_t index) {
	if (
		index >= windows.size() ||
		index == windows.size() - 1
		) {
		return;
	}

	auto selectedWindow =
		std::move(windows[index]);

	windows.erase(
		windows.begin() + index
	);

	windows.push_back(
		std::move(selectedWindow)
	);
}

void WindowManager::SelectTopOpenWindow() {
	activeWindow = nullptr;

	for (
		int i = static_cast<int>(windows.size()) - 1;
		i >= 0;
		i--
		) {
		if (windows[i]->IsOpen()) {
			activeWindow =
				windows[i].get();

			return;
		}
	}
}