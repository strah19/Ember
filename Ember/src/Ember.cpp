#include "Ember.h"

namespace Ember {
	SDL_DisplayMode GetDisplay() {
		SDL_DisplayMode display_mode;

		if (SDL_GetDesktopDisplayMode(0, &display_mode) != 0) {
			SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
			return display_mode;
		}

		return display_mode;
	}

	SDL_bool ConvertToSDLBool(bool value) {
		if (value)
			return SDL_TRUE;
		else
			return SDL_FALSE;
	}
}