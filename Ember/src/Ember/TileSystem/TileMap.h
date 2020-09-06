#ifndef TILE_MAP_H
#define TILE_MAP_H

#include "../Structures/FileManager.h"
#include "Grid.h"
#include "../Animation/SpriteSheet.h"

namespace ember {
	struct TileInfo {
		int texture_id;
	};

	class TileMapSerializer {
	public:
		TileMapSerializer(GridComponents& grid, const char* file_path);
		void Save(GridComponents& grid, std::vector<TileInfo>& info);

		std::vector<TileInfo> ReadTextureInfo(int col, int row);
	private:
		File serialier;
		int index;
		std::stringstream output;

		int col_count, row_count;

		void ClearStream();
		void Read(int& value);
		void PullSize(GridComponents& grid);
		void PullGrid(GridComponents& grid);
	};

	class TileMap : public Grid {
	public:
		TileMap(Renderer2D* renderer, Input* input, GridComponents& grid);

		void AddLayer(std::vector<TileInfo>& texture_info);
		void RenderTextures(SpriteSheet& sprite_sheet);
		void UploadTextureVector(std::vector<TileInfo>& texture_info, size_t layer_num);
	private:
		std::vector<std::vector<TileInfo>> layers_texture_info;
	};

	class TileMapEditor {
	public:
		TileMapEditor(TileMap& map);
		void UpdateMap(std::vector<TileInfo>& texture_info);
		void RenderSpriteSheet(SpriteSheet& sprite_sheet, const IVec2& start_location, int btn_id);
	private:
		TileMap map;
		int current_user_texture;
		Button texture_picker;
	};
}

#endif // !TILE_MAP_H
