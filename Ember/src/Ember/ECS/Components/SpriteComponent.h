#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "../../Structures/Texture.h"
#include "PositionComponent.h"

namespace ember {
	class SpriteComponent : public Component {
	public:
		SpriteComponent(const char* file_path, Renderer2D* renderer);
		virtual ~SpriteComponent() = default;

		void Init();
		void Render();
		void Update() { }

		void TextureColor(const Color& color) { texture.TextureColor(color); }
		void TextureAlptha(Uint8 a) { texture.TextureAlptha(a); }
		IVec2 GetTextureInfo() { texture.GetTextureInfo(); }

		void SetClip(const Rect& clip) { this->clip = clip; }
		void SetFlipMode(const SDL_RendererFlip& flip) { this->flip = flip; }
	private:
		Texture texture;
		Renderer2D* renderer;
		PositionComponent* position;
		Rect clip;
		SDL_RendererFlip flip;
	};
}

#endif // !SPRITE_COMPONENT_H
