#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include "../Structures/Texture.h"

namespace ember {
	class SpriteSheet {
	public:
		SpriteSheet(Texture& texture, int rows, int cols);

		void SelectSprite(int x, int y);
		Rect ReturnSourceRect();

		IVec2 Size() { return { col, row }; }
	private:
		Rect clip;
		int row, col;
	};
}

#endif // !SPRITE_SHEET_H
