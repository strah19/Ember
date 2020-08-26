#include "EmberPch.h"
#include "PositionComponent.h"

namespace ember {
	PositionComponent::PositionComponent(float x, float y, float w, float h)
		: angle(0.0f), position({ x, y, w, h }) {
	}

	PositionComponent::PositionComponent(const FRect& position)
		: angle(0.0f), position(position) {
	}

	PositionComponent::~PositionComponent() { }
	void PositionComponent::Init() { }
	void PositionComponent::Update() { }
	void PositionComponent::Render() { }

	void PositionComponent::ChangePosition(float x, float y) {
		position.x = x;
		position.y = y;
	}

	void PositionComponent::ChangeSize(float w, float h) {
		position.w = w;
		position.h = h;
	}

	void PositionComponent::UpdatePosition(float dx, float dy) {
		position.x += dx;
		position.y += dy;
	}

	void PositionComponent::UpdateSize(float dw, float dh) {
		position.w += dw;
		position.h += dh;
	}

	void PositionComponent::Angle(float angle) {
		angle = angle;
	}

	void PositionComponent::Scale(int on_x, int on_y) {
		position.w *= on_x;
		position.h *= on_y;
	}
}