#include "Cursor.h"

namespace Ember {
	Cursor::Cursor() {
		cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	}
	Cursor::~Cursor() {
		SDL_FreeCursor(cursor);
	}

	void Cursor::SetCursor(CursorType type) {
		this->type = type;
	}

	void Cursor::Update() {
		cursor = SDL_CreateSystemCursor((SDL_SystemCursor)type);
		SDL_SetCursor(cursor);
	}

	void Cursor::Display() {
		SDL_ShowCursor(SDL_ENABLE);
	}

	void Cursor::Hide() {
		SDL_ShowCursor(SDL_DISABLE);
	}
}