#ifndef ANIMATION_H
#define ANIMATION_H

#include "Animation/Spritesheet.h"

namespace Ember {
	class AnimationSource {
	public:
		virtual void DrawCurrentFrame(int& current_frame, const Rect& dest, float angle = 0.0f, const SDL_RendererFlip& flip = SDL_FLIP_NONE) = 0;
		int GetMaxSize() { return MAX_FRAMES; }
		void DefineStart(int s);
		void MaxFrames(int f) { MAX_FRAMES = f; }
	protected:
		int MAX_FRAMES = 0;
		int start = 0;
	};

	enum class SpriteSheetIterators {
		VERTICAL,
		HORIZONTAL, 
		NONE
	};

	class SpriteSheetToAnimation : public AnimationSource {
	public:
		void Initialize(SpriteSheet& sheet, int max_frames);
		void SetIterator(SpriteSheetIterators iterator);
		virtual void DrawCurrentFrame(int& current_frame, const Rect& dest, float angle = 0.0f, const SDL_RendererFlip& flip = SDL_FLIP_NONE);
	private:
		SpriteSheetIterators iterator = SpriteSheetIterators::NONE;
		SpriteSheet spritesheet;
	};

	class IndividualFramesToAnimation : public AnimationSource {
	public:
		void Initialize(const std::initializer_list<Texture>& textures);
		virtual void DrawCurrentFrame(int& current_frame, const Rect& dest, float angle = 0.0f, const SDL_RendererFlip& flip = SDL_FLIP_NONE);
	private:
		std::vector<Texture> textures;
	};

	class Animation {
	public:
		int Update(AnimationSource& source, const int frame_speed);
		bool HitFrame(int frame);
		void ResetFrame() {	current_frame = 0; }
		int& CurrentFrame() { return current_frame; }
		void FreezeFrame(bool freeze) { this->freeze = freeze; }
		bool FullCycle(AnimationSource& source) { return (CurrentFrame() == source.GetMaxSize() - 1); }
	private:
		int current_frame = 0;
		int current_point = 0;
		bool freeze = false;
	};
}

#endif // !ANIMATION_H
