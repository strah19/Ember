#include "Window.h"
#include "Assets/Assets.h"
#include "Assets/Texture.h"

namespace Ember {
	Window::Window(WindowProperties* properties)
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

	Window::~Window() {
		Destroy();
	}

	bool Window::Initializer(WindowProperties* properties) {
		bool initialize_checker = false;
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
			return initialize_checker;
		else
			initialize_checker = Create(properties);

		initialize_checker = InitializeAssets();

		return initialize_checker;
	}

	void Window::Destroy() {
		AssetCleanUp();
		SDL_DestroyWindow(native_window);
		SDL_Quit();

		properties = nullptr;
	}

	void Window::Update() {
		SDL_SetWindowSize(native_window, properties->width, properties->height);
		SDL_SetWindowTitle(native_window, properties->name.c_str());
		SDL_SetWindowFullscreen(native_window, properties->full_screen);
	}

	void Window::SetWindowIcon(const char* file_path) {
		SDL_Surface* icon = Texture::LoadSurface(file_path);
		SDL_SetWindowIcon(native_window, icon);
		SDL_FreeSurface(icon);
	}

	bool Window::AssertProperties() {
		return (properties != nullptr);
	}

	bool Window::Create(WindowProperties* properties) {
		if (AssertSize(properties->width, properties->height)) {
			CalculatePosition(&properties->position);

			native_window = SDL_CreateWindow(properties->name.c_str(), properties->position.x, properties->position.y, properties->width,
				properties->height, IsFullScreen(properties));
		}

		return (native_window != nullptr);
	}

	Uint32 Window::IsFullScreen(WindowProperties* properties) {
		if (properties->full_screen)
			return SDL_WINDOW_FULLSCREEN;
		return 0;
	}

	bool Window::AssertSize(uint32_t width, uint32_t height) {
		return (width != 0 && height != 0);
	}

	void Window::CalculatePosition(IVec2* position) {
		if (position->x == -1 && position->y == -1) {
			position->x = SDL_WINDOWPOS_CENTERED;
			position->y = SDL_WINDOWPOS_CENTERED;
		}
	}

	SDL_SysWMinfo GetSystemInfo(Window* window) {
		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);

		return info;
	}
}