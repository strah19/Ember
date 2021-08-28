#ifndef TEXTURE_H
#define TEXTURE_H

#include "Ember.h"

namespace Ember {
	class TextureLoader {
	public:
		static SDL_Surface* Load(const std::string& file_path);
		static void Free(SDL_Surface* surface);
		static void FlipVertically(SDL_Surface* surface);
	};
}

#endif // !TEXTURE_H
