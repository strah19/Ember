#include "TileMap.h"

namespace Ember {
	TileMapSerializer::TileMapSerializer(GridComponents& grid, const char* file_path)
		: serialier(file_path), index(0), col_count(0), row_count(0), gc_saved(false) {
		PullSize(grid);
		PullGrid(grid);

		Read(grid.start_x);
		Read(grid.start_y);
	}

	void TileMapSerializer::Save(GridComponents& grid, std::vector<TileInfo>& info) {
		if (!gc_saved) {
			serialier.EmptyFile();
			serialier.Write(grid.block_width);
			serialier.Write(" ");
			serialier.Write(grid.block_height);
			serialier.Write(" ");
			serialier.Write(grid.rows);
			serialier.Write(" ");
			serialier.Write(grid.cols);
			serialier.Write(" ");
			serialier.Write(grid.start_x);
			serialier.Write(" ");
			serialier.Write(grid.start_y);
			serialier.Write("\n");
			gc_saved = true;
		}
		for (int i = 0; i < grid.rows; i++) {
			for (int j = 0; j < grid.cols; j++) {
				serialier.Write(info[grid.cols * i + j].texture_id);
				serialier.Write(" ");
			}
			serialier.Write("\n");
		}
	}

	std::vector<TileInfo> TileMapSerializer::AddNewLayer(int col, int row) {
		serialier.Write("\n");
		std::vector<TileInfo> texture_ids;
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				serialier.Write(-1);
				serialier.Write(" ");
				texture_ids.push_back({ -1 });
			}
			serialier.Write("\n");
		}

		return texture_ids;
	}

	void TileMapSerializer::ReadTextureInfo(int col, int row, std::vector<TileInfo>& data) {
		unsigned int texture_id_num = col * row;
		serialier.DoEachWord([&](std::string& word, unsigned int counter) {
			if (counter < texture_id_num)
				data.push_back({ std::stoi(word) });

			return true;
			});
	}

	size_t TileMapSerializer::ReadLayeredTexture(int col, int row, std::vector<std::vector<TileInfo>>& data) {
		unsigned int texture_id_num = col * row;
		size_t current_layer = 0;
		data.push_back(std::vector<Ember::TileInfo>());
		serialier.DoEachWord([&](std::string& word, unsigned int counter) {
			if (counter >= texture_id_num) {
				data.push_back(std::vector<Ember::TileInfo>());
				texture_id_num += texture_id_num;
				current_layer++;
			}
			if (counter < texture_id_num)
				data[current_layer].push_back({ std::stoi(word) });

			return true;
			});

		return current_layer;
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

	TileMap::TileMap(rRenderer* renderer, Events* Events, const GridComponents& grid)
		: Grid(renderer, Events, grid) { }

	void TileMap::AddLayer(std::vector<TileInfo>& texture_info) {
		layers_texture_info.push_back(texture_info);
	}

	void TileMap::UploadTextureVector(std::vector<TileInfo>& texture_info, size_t layer_num) {
		if (layer_num < layers_texture_info.size())
			layers_texture_info[layer_num] = texture_info;
	}

	void TileMap::RenderTextures(SpriteSheet& sprite_sheet) {
		for (auto& layer : layers_texture_info) {
			x = grid.start_x - camera_position.x;
			y = grid.start_y - camera_position.y;
			for (int i = 0; i < grid.cols; i++) {
				for (int j = 0; j < grid.rows; j++) {
					if (layer[grid.cols * j + i].texture_id != -1) {
						sprite_sheet.SelectSprite(layer[grid.cols * j + i].texture_id % sprite_sheet.Size().x, layer[grid.cols * j + i].texture_id / sprite_sheet.Size().x);
						sprite_sheet.DrawSelectedSprite({ x, y, grid.block_width, grid.block_height });
					}
					y += grid.block_height;
				}
				x += grid.block_width;
				y = grid.start_y - camera_position.y;
			}
		}
	}

	TileMapEditor::TileMapEditor(TileMap& map)
		: map(map), current_user_texture(-1), texture_picker(map.Event(), { 0, 0, map.GridComponent().block_width, map.GridComponent().block_height }) { }

	void TileMapEditor::UpdateMap(std::vector<TileInfo>& texture_info, ButtonIds btn_id, const IVec2& camera_position) {
		IVec2 click_id = map.Hover();
		map.SetCameraPosition(camera_position);
		if (click_id.x != -1 && click_id.y != -1 && current_user_texture != -1 && map.Event()->ButtonId() == btn_id && map.Event()->Down())
			texture_info[map.GridComponent().cols * click_id.y + click_id.x].texture_id = current_user_texture;
	}

	void TileMapEditor::DeleteTile(std::vector<TileInfo>& texture_info, EmberKeyCode scancode) {
		IVec2 hover_on = map.Hover();
		if (map.Event()->KeyCode() == scancode)
			if (hover_on.x != -1 && hover_on.y != -1)
				texture_info[map.GridComponent().cols * hover_on.y + hover_on.x].texture_id = -1;
	}

	void TileMapEditor::RenderSpriteSheet(SpriteSheet& sprite_sheet, const IVec2& start_location, ButtonIds btn_id, const IVec2& size) {
		for (int i = 0; i < sprite_sheet.Size().x; i++) {
			for (int j = 0; j < sprite_sheet.Size().y; j++) {
				texture_picker.UpdatePosition(start_location.x + i * size.x, start_location.y + j * size.y);
				sprite_sheet.SelectSprite(i, j);
				sprite_sheet.DrawSelectedSprite({ start_location.x + i * size.x, start_location.y + j * size.y,
					size.x, size.y });
				if (texture_picker.Click(btn_id))
					current_user_texture = sprite_sheet.Size().x * j + i;
			}
		}
	}
}