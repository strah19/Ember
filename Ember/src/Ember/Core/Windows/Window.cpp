#include "Window.h"
#include "SDLWindow.h"
#include "OpenGLWindow.h"

namespace Ember {
	Window* Window::CreateEmberWindow(WindowProperties* properties) {
		return new SDLWindow(properties);
	}

	Window* Window::CreateEmberWindow(WindowProperties* properties, uint32_t major_opengl, uint32_t minor_opengl) {
		return new OpenGLWindow(properties, major_opengl, minor_opengl);
	}

	SDL_SysWMinfo GetSystemInfo() {
		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);

		return info;
	}
}