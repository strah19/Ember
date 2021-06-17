#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include <memory>
#include <string>

namespace EmberGL {
	class Texture {
	public:
		Texture(const char* file_path);
		Texture(uint32_t width, uint32_t height);

		virtual ~Texture();

		void Bind(uint32_t slot = 0);
		void UnBind();

		void SetData(void* data, uint32_t size);

		uint32_t GetWidth() const { return width; }
		uint32_t GetHeight() const { return height; }
		uint32_t GetTextureId() const { return texture_id; }
	private:
		uint32_t texture_id;

		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t internal_format = 0, data_format = 0;
		std::string path;
	};
}

#endif // !OPENGL_TEXTURE_H
