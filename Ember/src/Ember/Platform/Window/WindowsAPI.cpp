#include "WindowAPI.h"

namespace ember {
	void SetBorder(SDL_Window* window, SDL_bool border) { 
		SDL_SetWindowBordered(window, border); 
	}

	void SetBrightness(SDL_Window* window, float brightness) { 
		SDL_SetWindowBrightness(window, brightness);
	}

	void Rename(SDL_Window* window, const std::string& name) { 
		SDL_SetWindowTitle(window, name.c_str()); 
	}
}