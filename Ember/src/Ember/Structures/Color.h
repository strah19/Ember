#ifndef COLOR_H
#define COLOR_H

#include <SDL.h>

namespace ember {
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
}

#endif // !COLOR_H
