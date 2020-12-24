#ifndef TILE_MAP_H
#define TILE_MAP_H

#include "Ember.h"
#include "TileSystems/Grid.h"
#include "File/File.h"
#include "Animation/Spritesheet.h"
#include "StartUp/Events/KeyboardCodes.h"

namespace Ember {
	struct TileInfo {
		int texture_id;
	};

	class TileMapSerializer {
	public:
		TileMapSerializer(GridComponents& grid, const char* file_path);
		void Save(GridComponents& grid, std::vector<TileInfo>& info);

		void ReadTextureInfo(int col, int row, std::vector<TileInfo>& data);
		size_t ReadLayeredTexture(int col, int row, std::vector<std::vector<TileInfo>>& data);

		std::vector<TileInfo> AddNewLayer(int col, int row);

	private:
		File serialier;
		int index;
		std::stringstream output;

		int col_count, row_count;

		void ClearStream();
		void Read(int& value);
		void PullSize(GridComponents& grid);
		void PullGrid(GridComponents& grid);

		bool gc_saved;
	};

	class TileMap : public Grid {
	public:
		TileMap(rRenderer* renderer, Events* events, const GridComponents& grid);

		void AddLayer(std::vector<TileInfo>& texture_info);
		void RenderTextures(SpriteSheet& sprite_sheet);
		void UploadTextureVector(std::vector<TileInfo>& texture_info, size_t layer_num);
	private:
		std::vector<std::vector<TileInfo>> layers_texture_info;
	};

	class TileMapEditor {
	public:
		TileMapEditor(TileMap& map);
		void UpdateMap(std::vector<TileInfo>& texture_info, ButtonIds btn_id, const IVec2& camera_position);
		void RenderSpriteSheet(SpriteSheet& sprite_sheet, const IVec2& start_location, ButtonIds btn_id, const IVec2& size);
		void DeleteTile(std::vector<TileInfo>& texture_info, EmberKeyCode scancode);
	private:
		TileMap map;
		int current_user_texture;
		Button texture_picker;
	};
}

#endif // !TILE_MAP_H
