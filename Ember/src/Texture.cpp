#include "Texture.h"

namespace Ember {
	Texture::Texture(const std::string& file_path, rRenderer* renderer)
		: renderer(nullptr), texture(nullptr) {
		Initialize(file_path, renderer);
	}

	Texture::Texture() : renderer(nullptr), texture(nullptr) { }

	void Texture::Initialize(const std::string& file_path, rRenderer* renderer) {
		if (renderer != nullptr) {
			this->renderer = renderer;
			texture = LoadTexture(renderer->Renderer(), file_path);
			if (texture == nullptr) 
				std::cout << "ERROR: Could not create texture from file_path: " << file_path << std::endl;
		}
	}

	Texture::~Texture() {
		DeleteTexture();
	}

	void Texture::Draw(const FRect& dest, float angle, const SDL_RendererFlip& flip) {
		if(renderer != nullptr)
			SDL_RenderCopyExF(renderer->Renderer(), texture, nullptr, &dest.rect, angle, nullptr, flip);
	}

	void Texture::Draw(const FRect& dest, const Rect& src, const SDL_RendererFlip& flip, float angle) {
		if (renderer != nullptr)
			SDL_RenderCopyExF(renderer->Renderer(), texture, &src.rect, &dest.rect, angle, nullptr, flip);
	}

	void Texture::Draw(const Rect& dest, float angle, const SDL_RendererFlip& flip) {
		if (renderer != nullptr)
			SDL_RenderCopyEx(renderer->Renderer(), texture, nullptr, &dest.rect, angle, nullptr, flip);
	}

	void Texture::Draw(const Rect& dest, const Rect& src, const SDL_RendererFlip& flip, float angle) {
		if (renderer != nullptr)
			SDL_RenderCopyEx(renderer->Renderer(), texture, &src.rect, &dest.rect, angle, nullptr, flip);
	}

	SDL_Texture* Texture::LoadTexture(SDL_Renderer* renderer, const std::string& file_path) {
		SDL_Surface* surface = LoadSurface(file_path);

		return CreateTextureFromSurface(renderer, surface);
	}

	SDL_Texture* Texture::CreateTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface) {
		SDL_Texture* main_texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		return main_texture;
	}

	SDL_Surface* Texture::LoadSurface(const std::string& file_path) {
		SDL_Surface* surface = nullptr;
		surface = IMG_Load(file_path.c_str());
		if (surface == nullptr)
			return nullptr;

		return surface;
	}

	void Texture::DeleteTexture() {
		SDL_DestroyTexture(texture);
	}

	void Texture::TextureColor(const Color& color) {
		SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
	}

	void Texture::TextureAlptha(Uint8 alptha) {
		SDL_SetTextureAlphaMod(texture, alptha);
	}

	IVec2 Texture::GetTextureInfo() {
		int w, h;
		SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
		return { w, h };
	}
}