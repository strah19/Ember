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
		void DrawSelectedSprite(const Rect& rect);

		Rect ReturnSourceRect();

		IVec2 Size() { return { col, row }; }
	private:
		Rect clip;
		Texture texture;
		int row, col;
	};
}

#endif // !SPRITESHEET_H
