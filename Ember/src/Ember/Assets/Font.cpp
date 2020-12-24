#include "Font.h"

namespace Ember {
	Font::Font(rRenderer* renderer, const std::string& file_path, const char* text, int size, const Color& color, int x, int y) {
		Initialize(renderer, file_path, text, size, color, x, y);
	}

	Font::Font() : renderer(nullptr), font_width(0), font_height(0), font(nullptr), font_position({ 0, 0, 0, 0 }), font_texture(nullptr), font_color({ 0, 0, 0, 255 }), font_is_locked(true),
				   current_text_being_draw(" ") { }

	Font::~Font() {
		TTF_CloseFont(font);
	}

	void Font::Initialize(rRenderer* renderer, const std::string& file_path, const char* text, int size, const Color& color, int x, int y) {
		this->renderer = renderer;
		font_width = 0;
		font_height = 0;

		font = TTF_OpenFont(file_path.c_str(), size);
		font_position = { x, y, 0, 0 };
		font_texture = nullptr;
		font_color = color;

		current_text_being_draw = text;

		font_is_locked = true;
		if (font) {
			UnlockFont();
			UpdateFont();
		}
	}

	void Font::UnlockFont() {
		font_is_locked = false;
	}

	void Font::LockFont() {
		font_is_locked = true;
	}

	void Font::Render() {
		if (!font_is_locked && renderer != nullptr)
			SDL_RenderCopy(renderer->Renderer(), font_texture, nullptr, &font_position);
	}

	void Font::SetPosition(int x, int y) {
		if (!font_is_locked) {
			font_position.x = x;
			font_position.y = y;
		}
	}

	void Font::Translate(const Ember::IVec2& translation) {
		if (!font_is_locked) {
			font_position.x = font_position.x + translation.x;
			font_position.y = font_position.y + translation.y;
		}
	}

	Ember::IVec2 Font::GetSize() {
		int* w = &font_position.w;
		int* h = &font_position.h;
		TTF_SizeText(font, current_text_being_draw.c_str(), w, h);

		return { *w, *h };
	}

	void Font::UpdateFont() {
		if (!font_is_locked) {
			SDL_DestroyTexture(font_texture);
			SDL_Surface* fontSurface = TTF_RenderText_Blended(font, current_text_being_draw.c_str(), font_color.color);
			font_width = fontSurface->w;
			font_height = fontSurface->h;

			font_position.w = font_width;
			font_position.h = font_height;

			font_texture = SDL_CreateTextureFromSurface(renderer->Renderer(), fontSurface);

			SDL_FreeSurface(fontSurface);
		}
	}
}