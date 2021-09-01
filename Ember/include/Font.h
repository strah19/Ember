#ifndef FONT_H
#define FONT_H

#include "Texture.h"
#include <glm.hpp>
#include <map>

namespace Ember {
	struct Glyph {
		glm::ivec2 size;     
		glm::ivec2 bearing;   
		glm::ivec2 advance;
		float	   offset;
	};

	class Font {
	public:
		~Font();

		void Init(const char* filepath, uint32_t size);
		uint32_t GetSizeOfText(const std::string& text);

		uint32_t texture;
		uint32_t width = 0, height = 0;
		uint32_t size = 0;
		std::map<char, Glyph> glyphs;
	};
}

#endif // !FONT_H
