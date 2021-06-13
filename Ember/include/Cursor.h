#ifndef CURSOR_H
#define CURSOR_H

#include "Ember.h"

namespace Ember {
	enum class CursorType {
		Ibeam = SDL_SYSTEM_CURSOR_IBEAM, wait = SDL_SYSTEM_CURSOR_WAIT, crosshair = SDL_SYSTEM_CURSOR_CROSSHAIR,
		smallCursor = SDL_SYSTEM_CURSOR_WAITARROW, hand = SDL_SYSTEM_CURSOR_HAND, slashedCircle = SDL_SYSTEM_CURSOR_NO,
		fourPointedArrows = SDL_SYSTEM_CURSOR_SIZEALL, pointingNorthSouth = SDL_SYSTEM_CURSOR_SIZENS, pointingWestEast = SDL_SYSTEM_CURSOR_SIZEWE,
		pointingNorthWestToSouthEast = SDL_SYSTEM_CURSOR_SIZENWSE, pointingNorthEastToSouthWest = SDL_SYSTEM_CURSOR_SIZENESW, arrow = SDL_SYSTEM_CURSOR_ARROW
	};

	class Cursor {
	public:
		Cursor();
		~Cursor();

		void SetCursor(CursorType type);

		void Update();
		void Display();
		void Hide();
	private:
		SDL_Cursor* cursor = nullptr;
		CursorType type = CursorType::arrow;
	};
}

#endif // !CURSOR_H
