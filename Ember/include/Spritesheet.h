#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include "SDLTexture.h"

namespace Ember {
	class SpriteSheet {
	public:
		SpriteSheet(SDLTexture& texture, int rows, int cols);
		SpriteSheet();

		void Initialize(SDLTexture& texture, int rows, int cols);

		void SelectSprite(int x, int y);
		Rect ReturnSourceRect();

		IVec2 Size() { return { col, row }; }
		SDLTexture* GetTexture() { return &texture; }
	private:
		Rect clip;
		SDLTexture texture;
		int row, col;

	};

	class RandomAccessSpriteSheet {
	public:
		RandomAccessSpriteSheet(SDLTexture& texture, const std::initializer_list<Rect>& rects);
		RandomAccessSpriteSheet();

		void Initialize(SDLTexture& texture, const std::initializer_list<Rect>& rects);
		Rect ReturnSourceRect();
		void SelectSprite(size_t list_index);
		SDLTexture GetTexture() { return texture; }
	private:
		SDLTexture texture;
		size_t current_index = 0;
		std::vector<Rect> list_clips;
	};
}

#endif // !SPRITESHEET_H
