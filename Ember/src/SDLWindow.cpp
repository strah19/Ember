#include "SDLWindow.h"
#include "Assets.h"
#include "SDLTexture.h"

namespace Ember {
	SDLWindow::SDLWindow(WindowProperties* properties)
		: native_window(nullptr) {
		window_flags = 0;
		window_flags |= IsFullScreen(properties);
		is_running = Initializer(properties);
		if (is_running) {
			this->properties = properties;
			if (!AssertProperties())
				Destroy();
		}
		else
			Destroy();
	}

	SDLWindow::SDLWindow() 
		: native_window(nullptr) {
		window_flags = 0;
	}


	SDLWindow::~SDLWindow() {
		Destroy();
	}

	bool SDLWindow::Initializer(WindowProperties* properties) {
		bool initialize_checker = false;
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
			return initialize_checker;
		else
			initialize_checker = Create(properties);

		initialize_checker = InitializeAssets();

		return initialize_checker;
	}

	void SDLWindow::Destroy() {
		AssetCleanUp();
		SDL_DestroyWindow(native_window);
		SDL_Quit();

		properties = nullptr;
	}

	void SDLWindow::Update() {
		UpdateWindowAttributes();
	}

	void SDLWindow::UpdateWindowAttributes() {
		SDL_SetWindowSize(native_window, properties->width, properties->height);
		SDL_SetWindowTitle(native_window, properties->name.c_str());
		SDL_SetWindowFullscreen(native_window, properties->full_screen);
	}

	void SDLWindow::SetWindowIcon(const char* file_path) {
		SDL_Surface* icon = SDLTexture::LoadSurface(file_path);
		SDL_SetWindowIcon(native_window, icon);
		SDL_FreeSurface(icon);
	}

	bool SDLWindow::AssertProperties() {
		return (properties != nullptr);
	}

	bool SDLWindow::Create(WindowProperties* properties) {
		if (AssertSize(properties->width, properties->height)) {
			CalculatePosition(&properties->position);

			native_window = SDL_CreateWindow(properties->name.c_str(), properties->position.x, properties->position.y, properties->width,
				properties->height, window_flags);
		}

		return (native_window != nullptr);
	}

	Uint32 SDLWindow::IsFullScreen(WindowProperties* properties) {
		if (properties->full_screen)
			return SDL_WINDOW_FULLSCREEN_DESKTOP;
		return 0;
	}

	bool SDLWindow::AssertSize(uint32_t width, uint32_t height) {
		return (width != 0 && height != 0);
	}

	void SDLWindow::CalculatePosition(IVec2* position) {
		if (position->x == -1 && position->y == -1) {
			position->x = SDL_WINDOWPOS_CENTERED;
			position->y = SDL_WINDOWPOS_CENTERED;
		}
	}

}