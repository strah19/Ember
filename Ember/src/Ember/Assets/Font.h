#ifndef FONT_H
#define FONT_H

#include "Ember.h"
#include "Core/Renderer.h"

namespace Ember {
	class Font {
	public:
		Font(rRenderer* renderer, const std::string& file_path, int size);
		Font();
		~Font();

		void Initialize(rRenderer* renderer, const std::string& file_path, int size);
		void UnlockFont();

		void UpdateColor(const Color& color) { font_color = color; }
		template<typename T>
		void UpdateText(const T& text) {
			std::stringstream ss;
			ss << text;

			current_text_being_draw = ss.str();
		}

		void LockFont();
		void Render();
		void SetPosition(int x, int y);
		inline void SetSize(int w, int h) { font_width = w; font_height = h; }
		void Translate(const Ember::IVec2& translation);
		Ember::IVec2 GetSize();
		Ember::IVec2 GetSizeFromText(const std::string& text);

		enum Styles {
			Bold = TTF_STYLE_BOLD,
			Italic = TTF_STYLE_ITALIC,
			Underline = TTF_STYLE_UNDERLINE,
			Strike = TTF_STYLE_STRIKETHROUGH,
			Normal = TTF_STYLE_NORMAL
		};

		int GetMaxHeight() { return TTF_FontHeight(font); }
		void SetStyle(Styles style);

		const char* GetPath() { return file_path; }
		template <typename T>
		static void Draw(const T& data, const Color& color, const Ember::IVec2& position, Font* font) {
			font->UpdateText(data);
			font->SetPosition(position);
			font->UpdateColor(color);
			font->Render();
		}
	private:
		const char* file_path;
		void UpdateFont();
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
