/**
 * @file OpenGLWindow.cpp
 * @author strah19
 * @date October 3 2021
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the MIT License as
 * published by the Free Software Foundation.
 *
 * @section DESCRIPTION
 *
 * This file defines a window handler that also creates an OpenGL context.
 */

#include "OpenGLWindow.h"
#include "Logger.h"
#include "Config.h"

namespace Ember {
	OpenGLWindow::OpenGLWindow(WindowProperties* properties, uint32_t major_opengl, uint32_t minor_opengl) {
#ifndef EMBER_OPENGL_ACTIVATED
		EMBER_LOG_ERROR("To use OpenGLWindow, you must first load glad. This class will still work but OpenGL will not.");
#endif

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		if (major_opengl && minor_opengl) {
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major_opengl);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor_opengl);
		}
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