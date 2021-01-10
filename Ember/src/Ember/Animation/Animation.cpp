#include "Animation.h"

namespace Ember {
	void SpriteSheetToAnimation::Initialize(SpriteSheet& sheet, int max_frames) {
		spritesheet = sheet;
		MAX_FRAMES = max_frames;
	}

	void AnimationSource::DefineStart(int s) {
		if (s > -1)
			this->start = s;
	}

	void SpriteSheetToAnimation::SetIterator(SpriteSheetIterators iterator) {
		this->iterator = iterator;
	}

	void SpriteSheetToAnimation::DrawCurrentFrame(int& current_frame, const Rect& dest, float angle, const SDL_RendererFlip& flip) {
		if (current_frame + start > spritesheet.Size().x)
			current_frame = 0;
		switch (iterator) {
		case SpriteSheetIterators::HORIZONTAL: {
			spritesheet.SelectSprite(current_frame % spritesheet.Size().x, current_frame / spritesheet.Size().x);
			spritesheet.GetTexture()->Draw(dest, spritesheet.ReturnSourceRect(), flip, angle);
			break;
		}
		case SpriteSheetIterators::VERTICAL: {
			spritesheet.SelectSprite(current_frame / spritesheet.Size().x, current_frame % spritesheet.Size().x);
			spritesheet.GetTexture()->Draw(dest, spritesheet.ReturnSourceRect(), flip, angle);
			break;
		}
		case SpriteSheetIterators::NONE: {
			IVec2 selection = { (start + current_frame) % spritesheet.Size().x, (start + current_frame) / spritesheet.Size().x };
			spritesheet.SelectSprite(selection.x, selection.y);
			spritesheet.GetTexture()->Draw(dest, spritesheet.ReturnSourceRect(), flip, angle);
			break;
		}
		}
	}

	void IndividualFramesToAnimation::Initialize(const std::initializer_list<Texture>& textures) {
		this->textures = textures;
	}

	void IndividualFramesToAnimation::DrawCurrentFrame(int& current_frame, const Rect& dest, float angle, const SDL_RendererFlip& flip) {
		if ((size_t) (start + current_frame) > textures.size())
			current_frame = 0;

		textures[start + current_frame].Draw(dest, angle, flip);
	}

	int Animation::Update(AnimationSource& source, const int frame_speed) {
		if (!freeze) {
			if ((int)SDL_GetTicks() - current_point >= frame_speed) {
				current_point = SDL_GetTicks();
				if (source.GetMaxSize() != current_frame + 1)
					current_frame++;
				else
					ResetFrame();
			}
		}

		return current_frame;
	}

	bool Animation::HitFrame(int frame) {
		return (current_frame = frame);
	}
}