#ifndef WINDOW_API_H
#define WINDOW_API_H

#include "WindowsWindow.h"

namespace ember {
	void SetBorder(SDL_Window* window, SDL_bool border);
	void SetBrightness(SDL_Window* window, float brightness);
	void Rename(SDL_Window* window, const std::string& name);
}

#endif // !WINDOW_API_H
