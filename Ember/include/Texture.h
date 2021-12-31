#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include <memory>
#include <string>
#include <glm.hpp>

namespace Ember {
	class Texture {
	public:
		Texture() = default;

		void Init(const char* file_path);
		void Init(uint32_t width, uint32_t height);

		virtual ~Texture();

		void Bind(uint32_t slot = 0);
		void UnBind();

		void SetData(void* data);

		uint32_t GetWidth() const { return width; }
		uint32_t GetHeight() const { return height; }
		uint32_t GetTextureId() const { return texture_id; }
	private:
		uint32_t texture_id = 0;

		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t internal_format = 0, data_format = 0;
		std::string path;
	};

	struct Pixel {
		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;
	};

	void BindTexture(uint32_t id);
	void SetPixels(const glm::ivec2& position, void* pixels, const glm::ivec2& size = { 1, 1 });
	void GetPixels(const glm::ivec2& position, void* pixels, const glm::ivec2& size = { 1, 1 });
}

#endif // !OPENGL_TEXTURE_H
