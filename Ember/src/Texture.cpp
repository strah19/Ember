#include "Texture.h"
#include "TextureLoader.h"
#include "Logger.h"

#include <iostream>
#include <glad/glad.h>

#define RED 0
#define GREEN 1
#define BLUE 2

namespace Ember {
	Texture::Texture(const char* file_path, bool flip) {
		Init(file_path, flip);
	}

	Texture::Texture(uint32_t width, uint32_t height) {
		Init(width, height);
	}

	void Texture::Init(const char* file_path, bool flip) {
		path = file_path;
		SDL_Surface* s = Ember::TextureLoader::Load(file_path);
		if (flip)
			Ember::TextureLoader::FlipVertically(s);
		width = s->w;
		height = s->h;
		if (s->format->BytesPerPixel == 4) {
			internal_format = GL_RGBA8;
			data_format = GL_RGBA;
		}
		else if (s->format->BytesPerPixel == 3) {
			internal_format = GL_RGB8;
			data_format = GL_RGB;
		}

		if (s->pixels) {
			glCreateTextures(GL_TEXTURE_2D, 1, &texture_id);
			glTextureStorage2D(texture_id, 1, internal_format, width, height);

			glTextureParameteri(texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(texture_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureParameteri(texture_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(texture_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(texture_id, 0, 0, 0, width, height, data_format, GL_UNSIGNED_BYTE, s->pixels);
		}

		Ember::TextureLoader::Free(s);
	}

	void Texture::Init(uint32_t width, uint32_t height) {
		internal_format = GL_RGBA8;
		data_format = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &texture_id);
		glTextureStorage2D(texture_id, 1, internal_format, width, height);

		glTextureParameteri(texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(texture_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(texture_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(texture_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	Texture::~Texture() {
		glDeleteTextures(1, &texture_id);
	}

	void Texture::SetData(void* data) {
		uint32_t bpp = data_format == GL_RGBA ? 4 : 3;
		glTextureSubImage2D(texture_id, 0, 0, 0, width, height, data_format, GL_UNSIGNED_BYTE, data);
	}

	void Texture::Bind(uint32_t slot) {
		glBindTextureUnit(slot, texture_id);
	}

	void Texture::UnBind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void BindTexture(uint32_t id) {
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void SetPixels(const glm::ivec2& position, void* pixels, const glm::ivec2& size) {
		glTexSubImage2D(GL_TEXTURE_2D, 0, position.x, position.y, size.x, size.y, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	}

	void GetPixels(const glm::ivec2& position, void* pixels, const glm::ivec2& size) {
		glReadPixels(position.x, position.y, size.x, size.y, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	}
}

