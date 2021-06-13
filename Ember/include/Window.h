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
		WindowProperties(const std::string& name, int width, int height)
			: name(name), width(width), height(height), position(-1, -1), full_screen(false) { }
	};

	class Window {
	public:
		virtual ~Window() = default;

		virtual void* GetNativeWindow() = 0;
		virtual void Update() = 0;

		virtual inline void SetBorder(bool border) = 0;
		virtual inline void SetBrightness(float brightness) = 0;
		virtual void SetWindowIcon(const char* file_path) = 0;
		virtual inline void SetResizeable(bool resize) = 0;

		bool IsRunning() const { return is_running; }
		inline void Quit() { is_running = false; }

		WindowProperties* Properties() { return properties; }
		static Window* CreateEmberWindow(WindowProperties* properties);
		static Window* CreateEmberWindow(WindowProperties* properties, uint32_t major_opengl, uint32_t minor_opengl);
	protected:
		WindowProperties* properties;

		bool is_running = false;
	};

	SDL_SysWMinfo GetSystemInfo();
}

#endif // !WINDOWS_H
