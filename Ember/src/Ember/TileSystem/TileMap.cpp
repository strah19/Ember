#include "EmberPch.h"
#include "TileMap.h"

namespace ember {
	TileMapSerializer::TileMapSerializer(GridComponents& grid, const char* file_path)
		: serialier(file_path), index(0), col_count(0), row_count(0) {
		PullSize(grid);
		PullGrid(grid);	
	}

	void TileMapSerializer::Save(GridComponents& grid, std::vector<TileInfo>& info) {
		serialier.EmptyFile();
		serialier.Write(grid.block_width);
		serialier.Write(" ");
		serialier.Write(grid.block_height);
		serialier.Write(" ");
		serialier.Write(grid.rows);
		serialier.Write(" ");
		serialier.Write(grid.cols);
		serialier.Write("\n");

		for (int i = 0; i < grid.rows; i++) {
			for (int j = 0; j < grid.cols; j++) {
				serialier.Write(info[grid.cols * i + j].texture_id);
				serialier.Write(" ");
			}
			serialier.Write("\n");
		}
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

	void TileMap::UploadTextureVector(std::vector<TileInfo>& texture_info, size_t layer_num) {
		if (layer_num < layers_texture_info.size())	
			layers_texture_info[layer_num] = texture_info;
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

	TileMapEditor::TileMapEditor(TileMap& map) 
		: map(map), current_user_texture(-1), texture_picker(map.Inputs(), 0, 0, map.GridComponent().block_width, map.GridComponent().block_height) { }

	void TileMapEditor::UpdateMap(std::vector<TileInfo>& texture_info) {
		IVec2 click_id = map.Click(1);
		if (click_id.x != -1 && click_id.y != -1 && current_user_texture != -1) 
			texture_info[map.GridComponent().cols * click_id.y + click_id.x].texture_id = current_user_texture;
	}

	void TileMapEditor::RenderSpriteSheet(SpriteSheet& sprite_sheet, const IVec2& start_location, int btn_id) {
		for (int i = 0; i < sprite_sheet.Size().x; i++) {
			for (int j = 0; j < sprite_sheet.Size().y; j++) {
				texture_picker.UpdatePosition(start_location.x + i * map.GridComponent().block_width, start_location.y + j * map.GridComponent().block_height);
				sprite_sheet.SelectSprite(i, j);
				sprite_sheet.DrawSelectedSprite({ start_location.x + i * map.GridComponent().block_width, start_location.y + j * map.GridComponent().block_height,
					map.GridComponent().block_width, map.GridComponent().block_height });
				if (texture_picker.Click(btn_id)) 
					current_user_texture = sprite_sheet.Size().x * j + i;
			}
		}
	}
}