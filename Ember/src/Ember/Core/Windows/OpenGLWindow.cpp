#include "OpenGLWindow.h"

namespace Ember {
	OpenGLWindow::OpenGLWindow(WindowProperties* properties, uint32_t major_opengl, uint32_t minor_opengl) {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major_opengl);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor_opengl);

		is_running = Initializer(properties);
		AddWindowFlag(SDL_WINDOW_OPENGL);
		if (is_running) {
			this->properties = properties;
			if (!AssertProperties())
				Destroy();
		}
		else
			Destroy();

		glcontext = SDL_GL_CreateContext(native_window);
	}

	OpenGLWindow::~OpenGLWindow() {
		SDL_GL_DeleteContext(glcontext);
	}

	void OpenGLWindow::Update() {
		UpdateWindowAttributes();
		SDL_GL_SwapWindow(native_window);
	}
}