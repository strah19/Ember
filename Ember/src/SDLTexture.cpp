#include "SDLTexture.h"

namespace Ember {
	SDLTexture::SDLTexture(const std::string& file_path, rRenderer* renderer)
		: renderer(nullptr), texture(nullptr) {
		Initialize(file_path, renderer);
	}

	SDLTexture::SDLTexture() : renderer(nullptr), texture(nullptr) { }

	void SDLTexture::Initialize(const std::string& file_path, rRenderer* renderer) {
		if (renderer != nullptr) {
			this->renderer = renderer;
			texture = LoadTexture(renderer->Renderer(), file_path);
			if (texture == nullptr) 
				std::cout << "ERROR: Could not create texture from file_path: " << file_path << std::endl;
		}
	}

	SDLTexture::~SDLTexture() {
		DeleteTexture();
	}

	void SDLTexture::Draw(const FRect& dest, float angle, const SDL_RendererFlip& flip) {
		if(renderer != nullptr)
			SDL_RenderCopyExF(renderer->Renderer(), texture, nullptr, &dest.rect, angle, nullptr, flip);
	}

	void SDLTexture::Draw(const FRect& dest, const Rect& src, const SDL_RendererFlip& flip, float angle) {
		if (renderer != nullptr)
			SDL_RenderCopyExF(renderer->Renderer(), texture, &src.rect, &dest.rect, angle, nullptr, flip);
	}

	void SDLTexture::Draw(const Rect& dest, float angle, const SDL_RendererFlip& flip) {
		if (renderer != nullptr)
			SDL_RenderCopyEx(renderer->Renderer(), texture, nullptr, &dest.rect, angle, nullptr, flip);
	}

	void SDLTexture::Draw(const Rect& dest, const Rect& src, const SDL_RendererFlip& flip, float angle) {
		if (renderer != nullptr)
			SDL_RenderCopyEx(renderer->Renderer(), texture, &src.rect, &dest.rect, angle, nullptr, flip);
	}

	SDL_Texture* SDLTexture::LoadTexture(SDL_Renderer* renderer, const std::string& file_path) {
		SDL_Surface* surface = LoadSurface(file_path);

		return CreateTextureFromSurface(renderer, surface);
	}

	SDL_Texture* SDLTexture::CreateTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface) {
		SDL_Texture* main_texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		return main_texture;
	}

	SDL_Surface* SDLTexture::LoadSurface(const std::string& file_path) {
		SDL_Surface* surface = nullptr;
		surface = IMG_Load(file_path.c_str());
		if (surface == nullptr)
			return nullptr;

		return surface;
	}

	void SDLTexture::DeleteTexture() {
		SDL_DestroyTexture(texture);
	}

	void SDLTexture::TextureColor(const Color& color) {
		SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
	}

	void SDLTexture::TextureAlptha(Uint8 alptha) {
		SDL_SetTextureAlphaMod(texture, alptha);
	}

	IVec2 SDLTexture::GetTextureInfo() {
		int w, h;
		SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
		return { w, h };
	}
}