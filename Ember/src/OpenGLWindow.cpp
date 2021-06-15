#include "OpenGLWindow.h"
#include "Logger.h"

namespace Ember {
	OpenGLWindow::OpenGLWindow(WindowProperties* properties, uint32_t major_opengl, uint32_t minor_opengl) {
#ifndef EMBER_OPENGL_ACTIVATED
		EMBER_LOG_ERROR("To use OpenGLWindow, you must first load glad. This class will still work but OpenGL will not.");
#endif

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major_opengl);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor_opengl);

		SDL_GL_LoadLibrary(NULL);
		AddWindowFlag(SDL_WINDOW_OPENGL);

		is_running = Initializer(properties);
		if (is_running) {
			this->properties = properties;
			if (!AssertProperties())
				Destroy();
		}
		else
			Destroy();

		glcontext = SDL_GL_CreateContext(native_window);

#ifdef EMBER_OPENGL_ACTIVATED
		gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
#endif

		SDL_GL_SetSwapInterval(1);
	}

	OpenGLWindow::~OpenGLWindow() {
		SDL_GL_DeleteContext(glcontext);
	}

	void OpenGLWindow::Update() {
		UpdateWindowAttributes();
		SDL_GL_SwapWindow(native_window);
	}
}