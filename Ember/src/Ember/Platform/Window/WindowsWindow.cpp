#include "EmberPch.h"
#include "WindowsWindow.h"

namespace ember {
#ifdef EMBER_WINDOW_PLATFORM
	WindowsWindow::WindowsWindow(WindowProperties* properties) 
		: is_running(false), native_window(nullptr) {
		is_running = Initializer(properties);
		if (is_running) {
			this->properties = properties;
			if (!AssertProperties())
				Destroy();
		}
		else
			Destroy();
	}

	WindowsWindow::~WindowsWindow() {
		Destroy();
	}

	bool WindowsWindow::Initializer(WindowProperties* properties) {
		bool initialize_checker = false;
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 
			return initialize_checker;
		else 
			initialize_checker = CreateWindow(properties);

		return initialize_checker;
	}

	void WindowsWindow::Destroy() {
			SDL_DestroyWindow(native_window);
			SDL_Quit();

			properties = nullptr;
	}

	bool WindowsWindow::AssertProperties() {
		return (properties != nullptr);
	}

	bool WindowsWindow::CreateWindow(WindowProperties* properties) {
		if (AssertSize(properties->width, properties->height)) {
			CalculatePosition(&properties->position);

			native_window = SDL_CreateWindow(properties->name.c_str(), properties->position.x, properties->position.y, properties->width,
				properties->height, IsFullScreen(properties));
		}  

		return (native_window != nullptr);
	}

	Uint32 WindowsWindow::IsFullScreen(WindowProperties* properties) {
		if (properties->full_screen)
			return SDL_WINDOW_FULLSCREEN;
		return 0;
	}

	bool WindowsWindow::AssertSize(uint32_t width, uint32_t height) {
		return (width != 0 && height != 0);
	}

	void WindowsWindow::CalculatePosition(IVec2* position) {
		if (position->x == -1 && position->y == -1) {
			position->x = SDL_WINDOWPOS_CENTERED;
			position->y = SDL_WINDOWPOS_CENTERED;
		}
	}
#endif // EMBER_WINDOWS_PLATFORM
}