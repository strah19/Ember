#include "EmberPch.h"
#include "Entity.h"

namespace ember {
	Component::Component() {
		parent_entity = nullptr;
	}

	Component::~Component() { }

	void Component::Update() { }

	void Component::Render() { }

	void Component::Init() { }

	Entity::Entity() {
		component_set.set();
		for (size_t i = 0; i < component_array.size(); i++) {
			component_array[i] = nullptr;
		}
	}

	void Entity::Update() {
		for (auto& i : components) {
			i->Update();
		}
	}

	void Entity::Render() {
		for (auto& i : components) {
			i->Render();
		}
	}
}