#ifndef WINDOWS_WINDOW_H
#define WINDOWS_WINDOW_H

#include "../Window.h"
#include <SDL.h>
#include "../../PlatformDetection.h"

namespace ember {
#ifdef EMBER_WINDOW_PLATFORM
	class WindowsWindow : public Window {
	public:
		WindowsWindow(WindowProperties* properties);
		virtual ~WindowsWindow();

		virtual int GetWidth() const override { return properties->width; }
		virtual int GetHeight() const override { return properties->height; }

		virtual SDL_Window* GetNativeWindow() const override { return native_window; }

		virtual bool IsRunning() const override { return is_running; }
		void Quit() { is_running = false; }

		virtual WindowProperties* Properties() { return properties; }

	private:
		WindowProperties* properties;
		SDL_Window* native_window;

		bool is_running;

		bool Initializer(WindowProperties* properties);
		void Destroy();
		bool AssertProperties();

		bool CreateWindow(WindowProperties* properties);
		Uint32 IsFullScreen(WindowProperties* properties);
		bool AssertSize(uint32_t width, uint32_t height);
		void CalculatePosition(IVec2* position);
	};
#endif // EMBER_WINDOW_PLATFORM
}

#endif // !WINDOWS_WINDOW_H
