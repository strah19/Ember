#include "EmberPch.h"
#include "SpriteSheet.h"

namespace ember {
	SpriteSheet::SpriteSheet(Texture& texture, int rows, int cols) 
		: row(rows), col(cols), clip({ 0, 0, 0, 0 }), texture(texture) {
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