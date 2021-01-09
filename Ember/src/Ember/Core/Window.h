#ifndef WINDOWS_H
#define WINDOWS_H

#include "Ember.h"
#include "SDL_syswm.h"

namespace Ember {
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
		Window(WindowProperties* properties);
		virtual ~Window();

		virtual SDL_Window* GetNativeWindow() const { return native_window; }
		virtual void Update();

		inline void SetBorder(bool border) { SDL_SetWindowBordered(native_window, ConvertToSDLBool(border)); }
		inline void SetBrightness(float brightness) { SDL_SetWindowBrightness(native_window, brightness); }
		inline void SetWindowIcon(const char* file_path);
		inline void SetResizeable(bool resize) { SDL_SetWindowResizable(native_window, ConvertToSDLBool(resize)); }
		bool IsRunning() const { return is_running; }
		void Quit() { is_running = false; }

		virtual WindowProperties* Properties() { return properties; }

	private:
		WindowProperties* properties;
		SDL_Window* native_window;

		bool is_running;

		bool Initializer(WindowProperties* properties);
		void Destroy();
		bool AssertProperties();

		bool Create(WindowProperties* properties);
		Uint32 IsFullScreen(WindowProperties* properties);
		bool AssertSize(uint32_t width, uint32_t height);
		void CalculatePosition(IVec2* position);
	};

	SDL_SysWMinfo GetSystemInfo(Window* window);
}

#endif // !WINDOWS_H
