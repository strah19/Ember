#ifndef ASSETS_H
#define ASSETS_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "../../Renderer/2DRenderer.h"

namespace ember {
	class Font {
	public:
		Font(Renderer2D* renderer, const char* file_path, const char* text, int size, const Color& color, int x, int y);
		void UnlockFont();

		template <typename T>
		void ChangeFont(const T& text, const Color& color) {
			if (!font_is_locked) {
				std::stringstream ss;
				ss << text;
				font_color = color;
				SDL_DestroyTexture(font_texture);
				SDL_Surface* fontSurface = TTF_RenderText_Blended(font, ss.str().c_str(), font_color.color);
				font_width = fontSurface->w;
				font_height = fontSurface->h;

				font_position.w = font_width;
				font_position.h = font_height;

				font_texture = SDL_CreateTextureFromSurface(renderer->Renderer(), fontSurface);

				SDL_FreeSurface(fontSurface);
			}
		}

		void LockFont();
		void Render();
		void SetPosition(int x, int y);
		inline void SetSize(int w, int h) { font_width = w; font_height = h; }
		void Update(int vel_x, int vel_y);
		void CleanUp();
		inline void GetSize(int* w, int* h, const char* text) { TTF_SizeText(font, text, w, h); }
	private:
		int font_width;
		int font_height;
		TTF_Font* font;
		SDL_Rect font_position;
		SDL_Texture* font_texture;
		Color font_color;
		bool font_is_locked;
		Renderer2D* renderer;
	};

	class AudioChunk {
	public:
		AudioChunk(const char* file_path);
		void Play();
		void Volume(unsigned int volume);
		virtual ~AudioChunk();
	private:
		Mix_Chunk* m_chunk;
		unsigned int m_volume;
	};

	class AudioMusic {
	public:
		AudioMusic(const char* file_path);
		void Play();
		void Volume(unsigned int volume);
		virtual ~AudioMusic();
	private:
		Mix_Music* m_music;
		unsigned int m_volume;
	};
}

#endif // !ASSETS_H
