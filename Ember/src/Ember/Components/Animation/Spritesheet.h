#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include "Assets/Texture.h"

namespace Ember {
	class SpriteSheet {
	public:
		SpriteSheet(Texture& texture, int rows, int cols);
		SpriteSheet();

		void Initialize(Texture& texture, int rows, int cols);

		void SelectSprite(int x, int y);
		Rect ReturnSourceRect();

		IVec2 Size() { return { col, row }; }
		Texture* GetTexture() { return &texture; }
	private:
		Rect clip;
		Texture texture;
		int row, col;

	};

	class RandomAccessSpriteSheet {
	public:
		RandomAccessSpriteSheet(Texture& texture, const std::initializer_list<Rect>& rects);
		RandomAccessSpriteSheet();

		void Initialize(Texture& texture, const std::initializer_list<Rect>& rects);
		Rect ReturnSourceRect();
		void SelectSprite(size_t list_index);
		Texture GetTexture() { return texture; }
	private:
		Texture texture;
		size_t current_index = 0;
		std::vector<Rect> list_clips;
	};
}

#endif // !SPRITESHEET_H
