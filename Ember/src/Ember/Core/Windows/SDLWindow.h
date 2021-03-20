#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H

#include "Window.h"

namespace Ember {
	class SDLWindow : public Window {
	public:
		SDLWindow(WindowProperties* properties);
		SDLWindow();
		void* GetNativeWindow() { return static_cast<void*>(native_window); }

		virtual ~SDLWindow();
		virtual void Update();

		virtual inline void SetBorder(bool border) override { SDL_SetWindowBordered(native_window, ConvertToSDLBool(border)); }
		virtual inline void SetBrightness(float brightness) override { SDL_SetWindowBrightness(native_window, brightness); }
		virtual void SetWindowIcon(const char* file_path);
		virtual inline void SetResizeable(bool resize) override { SDL_SetWindowResizable(native_window, ConvertToSDLBool(resize)); }
		void AddWindowFlag(uint32_t flag) { window_flags |= flag; }
	protected:
		SDL_Window* native_window;

		bool Initializer(WindowProperties* properties);
		void Destroy();
		bool AssertProperties();

		bool Create(WindowProperties* properties);
		Uint32 IsFullScreen(WindowProperties* properties);
		bool AssertSize(uint32_t width, uint32_t height);
		void CalculatePosition(IVec2* position);

		void UpdateWindowAttributes();
	private:
		uint32_t window_flags;
	};
}

#endif // !SDL_WINDOW_H
