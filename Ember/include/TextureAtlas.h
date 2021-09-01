#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include "Texture.h"
#include <glm.hpp>
#include <vector>
#include <unordered_map>

#include "File.h"

#define SPRITE_COORD_SIZE 4

namespace Ember {
	class TextureAtlas {
	public:
		void Init(const char* texture_path, uint32_t cols, uint32_t rows);
		void Init(Texture* texture, uint32_t cols, uint32_t rows);

		glm::vec2* GetTexCoords(uint32_t col, uint32_t row, const glm::vec2& override_size = glm::vec2(0, 0));
		Texture* GetTexture() { return texture; }
		glm::vec2 CalculateSpriteCoordinate(const glm::vec2& pixel_point);
		static glm::vec2 CalculateSpriteCoordinate(const glm::vec2& pixel_point, uint32_t w, uint32_t h);
	private:
		Texture* texture;
		uint32_t cols, rows;
		glm::vec2 sprite_size;
		glm::vec2 rect_coordinates[SPRITE_COORD_SIZE];
	};

	using RandomAccessInfo = std::unordered_map<std::string, std::vector<glm::vec2>>;

	class RandomAccessTextureAtlasParser {
	public:
		void Init(const char* data_path);
		~RandomAccessTextureAtlasParser();

		void Serialize(RandomAccessInfo& tiles);
		RandomAccessInfo DeSerialize();
	private:
		File file;
		bool open = false;
	};

	class RandomAccessTextureAtlas {
	public:
		void Init(const char* texture_path, const char* data_path);
		void Init(Texture* texture, const char* data_path);

		~RandomAccessTextureAtlas();

		void LogEntries();
		Texture* GetTexture() { return texture; }
		void AddTile(const char* name, const std::vector<glm::vec2>& points);
		std::vector<glm::vec2>* GetTexCoords(const char* name);
	private:
		Texture* texture;
		RandomAccessInfo tiles;
		RandomAccessTextureAtlasParser parser;
	};
}

#endif // !TEXTURE_ATLAS_H
