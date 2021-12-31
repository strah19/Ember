#include "TextureAtlas.h"
#include "Logger.h"

namespace Ember {
	void TextureAtlas::Init(const char* texture_path, uint32_t cols, uint32_t rows) {
		texture = new Texture();
		texture->Init(texture_path);
		this->rows = rows;
		this->cols = cols;
		sprite_size = { texture->GetWidth() / this->cols, texture->GetHeight() / this->rows };
	}

	void TextureAtlas::Init(Texture* texture, uint32_t cols, uint32_t rows) {
		if (!texture) {
			EMBER_LOG_WARNING("Texture Atlas should not recieve nullptr textures!");
		}
		else {
			this->texture = texture;
			this->rows = rows;
			this->cols = cols;
			sprite_size = { texture->GetWidth() / this->cols, texture->GetHeight() / this->rows };
			EMBER_LOG("Size of sprite: %d, %d", sprite_size.x, sprite_size.y);
		}
	}

	glm::vec2 TextureAtlas::CalculateSpriteCoordinate(const glm::vec2& pixel_point) {
		return ((!texture || pixel_point.x > texture->GetWidth() || pixel_point.y > texture->GetHeight()) ? glm::vec2({ -1, -1 }) : (pixel_point / glm::vec2(texture->GetWidth(), texture->GetHeight())));
	}

	glm::vec2 TextureAtlas::CalculateSpriteCoordinate(const glm::vec2& pixel_point, uint32_t w, uint32_t h) {
		return ((pixel_point.x > w || pixel_point.y > h) ? glm::vec2({ -1, -1 }) : (pixel_point / glm::vec2(w, h)));
	}

	glm::vec2* TextureAtlas::GetTexCoords(uint32_t col, uint32_t row, const glm::vec2& override_size) {
		glm::vec2 start_point = { col * sprite_size.x, row * sprite_size.y };
		glm::vec2 size = sprite_size;
		if (override_size != glm::vec2(0, 0))
			size = sprite_size * override_size;
		float y_coord = texture->GetHeight() - (start_point.y + size.y);

		glm::vec2 frame[] = {
			{ start_point.x, y_coord },
			{ start_point.x + size.x, y_coord},
			{ start_point.x + size.x, y_coord + size.y },
			{ start_point.x, y_coord + size.y }
		};

		for (int i = 0; i < SPRITE_COORD_SIZE; i++) {
			glm::vec2 temp = CalculateSpriteCoordinate(frame[i]);
			if (temp != glm::vec2({ -1, -1 })) {
				rect_coordinates[i] = temp;
			}
		}

		return rect_coordinates;
	}

	void RandomAccessTextureAtlasParser::Init(const char* data_path) {
		file = File(data_path);

		open = file.is_open();
		if (!file.is_open()) {
			EMBER_LOG_ERROR("Random Access Texture Atlas Parser failed to open '%s' for reading.", data_path);
		}
	}

	RandomAccessTextureAtlasParser::~RandomAccessTextureAtlasParser() {
		file.close();
	}

	void RandomAccessTextureAtlasParser::Serialize(RandomAccessInfo& tiles) {
		if (open) {
			file.empty();
			for (auto& tile : tiles) {
				file.write(tile.first);
				file.write(" ");
				for (int i = 0; i < SPRITE_COORD_SIZE; i++) {
					file.write(tile.second[i].x);
					file.write(" ");
					file.write(tile.second[i].y);
					file.write("   ");
				}
				file.write("\n");
			}
		}
	}

	RandomAccessInfo RandomAccessTextureAtlasParser::DeSerialize() {
		if (open) {
			uint32_t point_counter = 0;
			std::vector<glm::vec2> temp_data;
			for (int i = 0; i < SPRITE_COORD_SIZE; i++)
				temp_data.push_back(glm::vec2(0, 0));
			std::string current_entry;
			RandomAccessInfo rai;
			file.do_each_word([&](const std::string& word, uint32_t counter) {
				if (!(counter % 9 == 0) && counter != 0) {
					if (point_counter % 2 == 0) {
						temp_data[point_counter / 2].x = std::stof(word);
						point_counter++;
					}
					else {
						temp_data[point_counter / 2].y = std::stof(word);
						point_counter++;
					}
				}
				if (counter % 8 == 0 && counter != 0) {
					rai[current_entry] = temp_data;
					point_counter = 0;
				}
				if (counter % 9 == 0) {
					current_entry = word;
				}

				return true;
			});

			return rai;
		}

		return RandomAccessInfo();
	}

	void RandomAccessTextureAtlas::Init(const char* texture_path, const char* data_path) {
		texture = new Texture();
		texture->Init(texture_path);
		parser.Init(data_path);
		tiles = parser.DeSerialize();
	}

	void RandomAccessTextureAtlas::Init(Texture* texture, const char* data_path) {
		if (!texture) {
			EMBER_LOG_WARNING("Texture Atlas should not recieve nullptr textures!");
		}
		else {
			this->texture = texture;
			parser.Init(data_path);
			tiles = parser.DeSerialize();
		}
	}

	void RandomAccessTextureAtlas::LogEntries() {
		for (auto& tile : tiles) {
			EMBER_LOG("Tile '%s' with data [%f, %f], [%f, %f], [%f, %f], [%f, %f].", tile.first.c_str(), 
				tile.second[0].x, tile.second[0].y,
				tile.second[1].x, tile.second[1].y,
				tile.second[2].x, tile.second[2].y,
				tile.second[3].x, tile.second[3].y);
		}
	}

	RandomAccessTextureAtlas::~RandomAccessTextureAtlas() {
		//parser.Serialize(tiles);
	}

	void RandomAccessTextureAtlas::AddTile(const char* name, const std::vector<glm::vec2>& points) {
		tiles[name] = points;
	}

	std::vector<glm::vec2>* RandomAccessTextureAtlas::GetTexCoords(const char* name) {
		return (tiles.find(name) == tiles.end()) ? nullptr : &tiles[name];
	}
}