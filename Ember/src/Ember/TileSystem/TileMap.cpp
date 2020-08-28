#include "EmberPch.h"
#include "TileMap.h"

namespace ember {
	TileMapSerializer::TileMapSerializer(GridComponents& grid, const char* file_path)
		: serialier(file_path), index(0), col_count(0), row_count(0) {
		PullSize(grid);
		PullGrid(grid);	
	}

	void TileMapSerializer::Save(std::vector<TileInfo>& info) {
		serialier.EmptyFile();
		
	}

	std::vector<TileInfo> TileMapSerializer::ReadTextureInfo(int col, int row) {
		std::vector<TileInfo> texture_ids;
		unsigned int texture_id_num = col * row;
		serialier.DoEachWord([&](std::string& word, unsigned int counter) {
			if (counter < texture_id_num)
				texture_ids.push_back({ std::stoi(word) });
		});

		return texture_ids;
	}

	void TileMapSerializer::ClearStream() {
		output.str(std::string());
		output.clear();
	}

	void TileMapSerializer::Read(int& value) {
		ClearStream();

		output << serialier.ReadWord(index);
		output >> value;
		index++;
	}

	void TileMapSerializer::PullSize(GridComponents& grid) {
		Read(grid.block_width);
		Read(grid.block_height);
	}

	void TileMapSerializer::PullGrid(GridComponents& grid) {
		Read(grid.rows);
		row_count = grid.rows;
		Read(grid.cols);
		col_count = grid.cols;
	}

	TileMap::TileMap(Renderer2D* renderer, Input* input, GridComponents& grid)
		: Grid(renderer, input, grid) { }

	void TileMap::AddLayer(std::vector<TileInfo>& texture_info) {
		layers_texture_info.push_back(texture_info);
	}

	void TileMap::RenderTextures(SpriteSheet& sprite_sheet) {
		for (auto& layer : layers_texture_info) {
			x = grid.start_x;
			y = grid.start_y;
			for (int i = 0; i < grid.cols; i++) {
				for (int j = 0; j < grid.rows; j++) {
					if (layer[grid.cols * j + i].texture_id != -1) {
						sprite_sheet.SelectSprite(layer[grid.cols * j + i].texture_id % sprite_sheet.Size().x, layer[grid.cols * j + i].texture_id / sprite_sheet.Size().x);
						sprite_sheet.DrawSelectedSprite({ x, y, grid.block_width, grid.block_height });
					}
					y += grid.block_height;
				}
				x += grid.block_width;
				y = grid.start_y;
			}
		}
	}
}