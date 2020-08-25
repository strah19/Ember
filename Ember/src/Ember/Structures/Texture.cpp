#include "EmberPch.h"
#include "Texture.h"

namespace ember {
	Texture::Texture(const char* file_path, Renderer2D* renderer) 
		: renderer(nullptr), texture(nullptr) {
		if (renderer != nullptr) {
			this->renderer = renderer;
			texture = LoadTexture(renderer->Renderer(), file_path);
		}
	}

	Texture::~Texture() {
		DeleteTexture();
	}

	void Texture::Draw(const SDL_FRect& dest, float angle, const SDL_RendererFlip& flip) {
		SDL_RenderCopyExF(renderer->Renderer(), texture, NULL, &dest, angle, NULL, flip);
	}

	void Texture::Draw(const SDL_FRect& dest, const SDL_Rect& src, const SDL_RendererFlip& flip, float angle) {
		SDL_RenderCopyExF(renderer->Renderer(), texture, &src, &dest, angle, NULL, flip);
	}

	void Texture::Draw(const SDL_Rect& dest, float angle, const SDL_RendererFlip& flip) {
		SDL_RenderCopyEx(renderer->Renderer(), texture, NULL, &dest, angle, NULL, flip);
	}

	void Texture::Draw(const SDL_Rect& dest, const SDL_Rect& src, const SDL_RendererFlip& flip, float angle) {
		SDL_RenderCopyEx(renderer->Renderer(), texture, &src, &dest, angle, NULL, flip);
	}

	SDL_Texture* Texture::LoadTexture(SDL_Renderer* renderer, const char* file_path) {
		if (file_path == nullptr)
			return nullptr;

		SDL_Surface* surface = LoadSurface(file_path);

		return CreateTextureFromSurface(renderer, surface);
	}

	SDL_Texture* Texture::CreateTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface) {
		SDL_Texture* main_texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		return main_texture;
	}

	SDL_Surface* Texture::LoadSurface(const char* file_path) {
		SDL_Surface* Surface = nullptr;
		Surface = IMG_Load(file_path);
		if (Surface == nullptr)
			return nullptr;

		return Surface;
	}

	void Texture::DeleteTexture() {
		SDL_DestroyTexture(texture);
	}

	void Texture::TextureColor(const Color& color) {
		SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
	}

	void Texture::TextureAlptha(Uint8 a) {
		SDL_SetTextureAlphaMod(texture, a);
	}

	IVec2 Texture::GetTextureInfo() {
		int w, h;
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		return { w, h };
	}
}