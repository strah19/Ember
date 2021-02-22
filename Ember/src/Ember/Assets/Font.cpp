#include "Font.h"

namespace Ember {
	Font::Font(rRenderer* renderer, const std::string& file_path, int size) {
		Initialize(renderer, file_path, size);
	}

	Font::Font() : renderer(nullptr), font_width(0), font_height(0), font(nullptr), font_position({ 0, 0, 0, 0 }), font_texture(nullptr), font_color({ 0, 0, 0, 255 }), font_is_locked(true),
		current_text_being_draw(" ") { }

	Font::~Font() {
		TTF_CloseFont(font);
	}

	void Font::Initialize(rRenderer* renderer, const std::string& file_path, int size) {
		this->renderer = renderer;
		font_width = 0;
		font_height = 0;
		this->file_path = file_path.c_str();

		font = TTF_OpenFont(file_path.c_str(), size);
		font_position = { 0, 0, 0, 0 };
		font_texture = nullptr;
		font_color = { 0, 0, 0, 255 };

		current_text_being_draw = " ";

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
		if (!font_is_locked && renderer != nullptr) {
			UpdateFont();
			SDL_RenderCopy(renderer->Renderer(), font_texture, nullptr, &font_position);
		}
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

	Ember::IVec2 Font::GetSizeFromText(const std::string& text) {
		int* w = &font_position.w;
		int* h = &font_position.h;
		TTF_SizeText(font, text.c_str(), w, h);

		return { *w, *h };
	}

	void Font::UpdateFont() {
		if (!font_is_locked && current_text_being_draw != "") {
			SDL_DestroyTexture(font_texture);
			SDL_Surface* font_surface = TTF_RenderText_Blended(font, current_text_being_draw.c_str(), font_color.color);
			font_width = font_surface->w;
			font_height = font_surface->h;

			font_position.w = font_width;
			font_position.h = font_height;

			font_texture = SDL_CreateTextureFromSurface(renderer->Renderer(), font_surface);

			SDL_FreeSurface(font_surface);
		}
	}

	void Font::SetStyle(Styles style) {
		TTF_SetFontStyle(font, style);
	}
}