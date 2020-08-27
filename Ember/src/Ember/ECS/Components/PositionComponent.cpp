#include "EmberPch.h"
#include "PositionComponent.h"

namespace ember {
	PositionComponent::PositionComponent(int x, int y, int w, int h)
		: angle(0.0f), position({ x, y, w, h }) {
	}

	PositionComponent::PositionComponent(const Rect& position)
		: angle(0.0f), position(position) {
	}

	PositionComponent::~PositionComponent() { }
	void PositionComponent::Init() { }
	void PositionComponent::Update() { }
	void PositionComponent::Render() { }

	void PositionComponent::ChangePosition(int x, int y) {
		position.x = x;
		position.y = y;
	}

	void PositionComponent::ChangeSize(int w, int h) {
		position.w = w;
		position.h = h;
	}

	void PositionComponent::UpdatePosition(int dx, int dy) {
		position.x += dx;
		position.y += dy;
	}

	void PositionComponent::UpdateSize(int dw, int dh) {
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