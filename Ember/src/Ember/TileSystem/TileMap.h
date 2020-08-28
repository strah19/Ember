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
		void Save(std::vector<TileInfo>& info);

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
	private:
		std::vector<std::vector<TileInfo>> layers_texture_info;
	};
}

#endif // !TILE_MAP_H
