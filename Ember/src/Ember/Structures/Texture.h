#ifndef TEXTURE_H
#define TEXTURE_H

#include "../Renderer/2DRenderer.h"
#include "Vec2.h"
#include "Color.h"
#include "Rect.h"

namespace ember {
	class Texture {
	public:
		Texture(const char* file_path, Renderer2D* renderer);
		~Texture();

		void Draw(const FRect& dest, float angle = 0.0f, const SDL_RendererFlip& flip = SDL_FLIP_NONE);
		void Draw(const FRect& dest, const Rect& src, const SDL_RendererFlip& flip = SDL_FLIP_NONE, float angle = 0.0f);
		void Draw(const Rect& dest, float angle = 0.0f, const SDL_RendererFlip& flip = SDL_FLIP_NONE);
		void Draw(const Rect& dest, const Rect& src, const SDL_RendererFlip& flip = SDL_FLIP_NONE, float angle = 0.0f);

		static SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* file_path);
		static SDL_Surface* LoadSurface(const char* file_path);
		static SDL_Texture* CreateTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface);

		void DeleteTexture();

		void TextureColor(const Color& color);
		void TextureAlptha(Uint8 a);
		IVec2 GetTextureInfo();
	private:
		SDL_Texture* texture;
		Renderer2D* renderer;
	};
}

#endif // !TEXTURE_H
