#ifndef FONT_H
#define FONT_H

#include "Ember.h"
#include "StartUp/Renderer.h"

namespace Ember {
	class Font {
	public:
		Font(rRenderer* renderer, const std::string& file_path, const char* text, int size, const Color& color, int x, int y);
		Font();
		~Font();

		void Initialize(rRenderer* renderer, const std::string& file_path, const char* text, int size, const Color& color, int x, int y);
		void UnlockFont();
		void UpdateFont();

		void UpdateColor(const Color& color) { font_color = color; }
		template<typename T>
		void UpdateText(const T& text) { 
			std::stringstream ss;
			ss.str(std::string);
			ss << text;

			current_text_being_draw = ss.str();
		}

		void LockFont();
		void Render();
		void SetPosition(int x, int y);
		inline void SetSize(int w, int h) { font_width = w; font_height = h; }
		void Translate(const Ember::IVec2& translation);
		Ember::IVec2 GetSize();
	private:
		int font_width;
		int font_height;
		TTF_Font* font;
		SDL_Rect font_position;
		SDL_Texture* font_texture;
		Color font_color;
		bool font_is_locked;
		rRenderer* renderer;

		std::string current_text_being_draw;
	};
}

#endif // !FONT_H
