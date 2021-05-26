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

	Rect SpriteSheet::ReturnSourceRect() {
		return clip;
	}

	RandomAccessSpriteSheet::RandomAccessSpriteSheet(Texture& texture, const std::initializer_list<Rect>& rects) {
		Initialize(texture, rects);
	}

	RandomAccessSpriteSheet::RandomAccessSpriteSheet() { }

	void RandomAccessSpriteSheet::Initialize(Texture& texture, const std::initializer_list<Rect>& rects) {
		this->texture = texture;

		list_clips = rects;
	}

	Rect RandomAccessSpriteSheet::ReturnSourceRect() {
		if (current_index < list_clips.size())
			return list_clips[current_index];
		return { 0, 0, 0, 0 };
	}

	void RandomAccessSpriteSheet::SelectSprite(size_t list_index) {
		if (list_index > -1)
			current_index = list_index;
	}
}