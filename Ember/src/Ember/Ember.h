#ifndef EMBER_H
#define EMBER_H

#include <functional>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <sstream>

#include "Vector.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#define EMBER_BIND_FUNC(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Ember {
	struct Rect {
		union {
			struct {
				int x, y, w, h;
			};
			struct {
				IVec2 pos, size;
			};
			struct {
				SDL_Rect rect;
			};
		};
	};

	struct FRect {
		union {
			struct {
				float x, y, w, h;
			};
			struct {
				Vec2 pos, size;
			};
			struct {
				SDL_FRect rect;
			};
		};
	};

	struct Color {
		union {
			struct {
				Uint8 r, g, b, a;
			};
			struct {
				SDL_Color color;
			};
		};
	};

	SDL_DisplayMode GetDisplay();
	SDL_bool ConvertToSDLBool(bool value);
	void CheckVersion();
}

#endif // !EMBER_H
