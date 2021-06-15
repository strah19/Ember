#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#include "SDLWindow.h"

#ifdef EMBER_OPENGL_ACTIVATED
#include <glad/glad.h>
#endif

#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>

namespace Ember {
	class OpenGLWindow : public SDLWindow {
	public:
		OpenGLWindow(WindowProperties* properties, uint32_t major_opengl, uint32_t minor_opengl);
		virtual ~OpenGLWindow();

		virtual void Update() override;
	private:
		SDL_GLContext glcontext;
	};
}

#endif // !OPENGL_WINDOW_H
