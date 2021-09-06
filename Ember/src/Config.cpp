#include "Config.h"
#include <SDL.h>
#include <glad/glad.h>
#include "Logger.h"

namespace Ember {
	const char* OpenGLVersion() {
		return (const char*) glGetString(GL_VERSION);
	}

	void SDLVersion() {
		SDL_version compiled;
		SDL_version linked;

		SDL_VERSION(&compiled);
		SDL_GetVersion(&linked);
		EMBER_LOG_GOOD("We compiled against SDL version: %d%d%d", compiled.major, compiled.minor, compiled.patch);
		EMBER_LOG_GOOD("But we are linking against SDL version: %d%d%d", linked.major, linked.minor, linked.patch);
	}
}