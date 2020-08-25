#ifndef WINDOWS_H
#define WINDOWS_H

#include <string>
#include "Structures/Vec2.h"
#include <SDL.h>

namespace ember {
	struct WindowProperties {
		std::string name;
		int width;
		int height;
		bool full_screen;
		IVec2 position;
		WindowProperties()
			: name(), width(0), height(0), position(-1, -1), full_screen(false) { }
		WindowProperties(std::string name, int width, int height)
			: name(name), width(width), height(height), position(-1, -1), full_screen(false) { }
	};

	class Window {
	public:
		virtual ~Window() = default;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;

		virtual SDL_Window* GetNativeWindow() const = 0;
		virtual bool IsRunning() const = 0;
		virtual void Quit() = 0;

		virtual WindowProperties* Properties() = 0;
	};
}

#endif // !WINDOWS_H
