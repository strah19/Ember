#include "Spritesheet.h"

namespace Ember {
	SpriteSheet::SpriteSheet(Texture& texture, int rows, int cols) : clip({ 0, 0, 0, 0 }) {
		Initialize(texture, rows, cols);
	}

	SpriteSheet::SpriteSheet() : row(0), col(0), clip({0, 0, 0, 0 }) { }

	void SpriteSheet::Initialize(Texture& texture, int rows, int cols) {
		this->texture = texture;
		row = rows;
		col = cols;

		clip.size = texture.GetTextureInfo();

		clip.w /= col;
		clip.h /= row;
	}

	void SpriteSheet::SelectSprite(int x, int y) {
		clip.x = x * clip.w;
		clip.y = y * clip.h;
	}

	void SpriteSheet::DrawSelectedSprite(const Rect& rect) {
		texture.Draw(rect, clip);
	}

	Rect SpriteSheet::ReturnSourceRect() {
		return clip;
	}
}