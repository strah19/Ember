#ifndef RECT_H
#define RECT_H

#include "Vec2.h"

#include <SDL.h>

namespace ember {
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
}

#endif // !RECT_H
