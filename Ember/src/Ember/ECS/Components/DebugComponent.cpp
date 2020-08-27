#include "EmberPch.h"
#include "DebugComponent.h"

namespace ember {
	DebugComponent::DebugComponent(Input* input)
		: input(input), button(input, 0, 0, 0, 0) {

	}

	void DebugComponent::Init() {
		position = &parent_entity->GetComponent<PositionComponent>();
	}

	void DebugComponent::Render() { }

	void DebugComponent::Update() {
		button.UpdatePosition(position->Position().x, position->Position().y);
		button.UpdateSize(position->Position().size);

		if (button.Click(1)) {
			if (!moving) {
				moving = true;
				px = input->MousePosition().x - position->Position().x;
				py = input->MousePosition().y - position->Position().y;
			}
		}
		else if (moving) 
			position->ChangePosition(input->MousePosition().x - px, input->MousePosition().y - py);
		if (!input->Down()) 
			moving = false;
	}
}
