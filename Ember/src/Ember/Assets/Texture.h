#ifndef TEXTURE_H
#define TEXTURE_H

#include "Ember.h"
#include "StartUp/Renderer.h"

namespace Ember {
	class Texture {
	public:
		Texture(const std::string& file_path, rRenderer* renderer);
		Texture();

		~Texture();
		void Initialize(const std::string& file_path, rRenderer* renderer);

		void Draw(const FRect& dest, float angle = 0.0f, const SDL_RendererFlip& flip = SDL_FLIP_NONE);
		void Draw(const FRect& dest, const Rect& src, const SDL_RendererFlip& flip = SDL_FLIP_NONE, float angle = 0.0f);
		void Draw(const Rect& dest, float angle = 0.0f, const SDL_RendererFlip& flip = SDL_FLIP_NONE);
		void Draw(const Rect& dest, const Rect& src, const SDL_RendererFlip& flip = SDL_FLIP_NONE, float angle = 0.0f);

		static SDL_Texture* LoadTexture(SDL_Renderer* renderer, const std::string& file_path);
		static SDL_Surface* LoadSurface(const std::string& file_path);
		static SDL_Texture* CreateTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface);

		void DeleteTexture();

		void TextureColor(const Color& color);
		void TextureAlptha(Uint8 alptha);
		IVec2 GetTextureInfo();
	private:
		SDL_Texture* texture;
		rRenderer* renderer;
	};
}

#endif // !TEXTURE_H
