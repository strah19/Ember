/**
 * @file Texture.cpp
 * @author strah19
 * @date October 3 2021
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the MIT License as
 * published by the Free Software Foundation.
 *
 * @section DESCRIPTION
 *
 * This file represents a Texture object that can be rendered in Ember.
 */

#include "Texture.h"
#include "Logger.h"

#include <iostream>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Ember {
	/**
	* 
	* 
	*/
	void Texture::Init(const char* file_path) {
		path = file_path;

		int w, h, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(file_path, &w, &h, &channels, 0);

		width = w;
		height = h;
		if (channels == 4) {
			internal_format = GL_RGBA8;
			data_format = GL_RGBA;
		}
		else if (channels == 3) {
			internal_format = GL_RGB8;
			data_format = GL_RGB;
		}

		if (data) {
			glCreateTextures(GL_TEXTURE_2D, 1, &texture_id);
			glTextureStorage2D(texture_id, 1, internal_format, width, height);

			glTextureParameteri(texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(texture_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureParameteri(texture_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(texture_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(texture_id, 0, 0, 0, width, height, data_format, GL_UNSIGNED_BYTE, data);
		}
		stbi_image_free(data);
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

