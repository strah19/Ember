#include "Ember.h"

namespace Ember {
	void CheckVersion() {
		SDL_version compiled;
		SDL_version linked;

		SDL_VERSION(&compiled);
		SDL_GetVersion(&linked);
		std::cout << "We compiled against SDL version: " << compiled.major << compiled.minor << compiled.patch << std::endl;
		std::cout << "But we are linking against SDL version: " << linked.major << linked.minor << linked.patch << std::endl;
	}

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