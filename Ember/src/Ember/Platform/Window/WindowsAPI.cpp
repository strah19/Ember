#include "EmberPch.h"
#include "WindowAPI.h"
#include "../../Structures/Texture.h"

namespace ember {
	void SetBorder(SDL_Window* window, SDL_bool border) { 
		SDL_SetWindowBordered(window, border); 
	}

	void SetBrightness(SDL_Window* window, float brightness) { 
		SDL_SetWindowBrightness(window, brightness);
	}

	void Rename(SDL_Window* window, const std::string& name) { 
		SDL_SetWindowTitle(window, name.c_str()); 
	}

	void SetWindowIcon(SDL_Window* window, const char* file_path) {
		SDL_Surface* icon = Texture::LoadSurface(file_path);
		SDL_SetWindowIcon(window, icon);
		SDL_FreeSurface(icon);
	}
}