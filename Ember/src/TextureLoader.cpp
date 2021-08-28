#include "TextureLoader.h"
#include "Logger.h"

namespace Ember {
	SDL_Surface* TextureLoader::Load(const std::string& file_path) {
		SDL_Surface* surface = nullptr;
		surface = IMG_Load(file_path.c_str());
		if (surface == nullptr) {
			EMBER_LOG_ERROR("Failed to load '%s'.", file_path.c_str());
			return nullptr;
		}
		else
			EMBER_LOG_GOOD("Loaded '%s'.", file_path.c_str());

		return surface;
	}

	void TextureLoader::Free(SDL_Surface* surface) {
		SDL_FreeSurface(surface);
	}

	void TextureLoader::FlipVertically(SDL_Surface* surface) {
		SDL_LockSurface(surface);

		int pitch = surface->pitch; // row size
		char* temp = new char[pitch]; // intermediate buffer
		char* pixels = (char*)surface->pixels;

		for (int i = 0; i < surface->h / 2; ++i) {
			// get pointers to the two rows to swap
			char* row1 = pixels + i * pitch;
			char* row2 = pixels + (surface->h - i - 1) * pitch;

			// swap rows
			memcpy(temp, row1, pitch);
			memcpy(row1, row2, pitch);
			memcpy(row2, temp, pitch);
		}

		delete[] temp;

		SDL_UnlockSurface(surface);
	}
}