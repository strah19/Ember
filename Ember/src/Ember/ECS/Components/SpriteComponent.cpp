#include "EmberPch.h"
#include "SpriteComponent.h"

namespace ember {
	SpriteComponent::SpriteComponent(const char* file_path, Renderer2D* renderer)
		: renderer(renderer), texture(file_path, renderer), clip({ 0, 0, texture.GetTextureInfo().x, texture.GetTextureInfo().y }), flip(SDL_FLIP_NONE) {
	}

	void SpriteComponent::Init() {
		position = &parent_entity->GetComponent<PositionComponent>();
	}

	void SpriteComponent::Render() {
		texture.Draw(position->Position(), clip, flip, position->GetAngle());
	}
}