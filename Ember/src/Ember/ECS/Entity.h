#ifndef ENTITY_H
#define ENTITY_H

#include "../Structures/Rect.h"

#include <bitset>
#include <array>

namespace ember {
	class Component;
	class Entity;

	using ComponentID = std::size_t;

	inline ComponentID GetComponentTypeID() {
		static ComponentID current_component_id = 0;
		return current_component_id++;
	}

	template <typename T> inline ComponentID GetComponentTypeID() {
		static ComponentID typeID = GetComponentTypeID();
		return typeID;
	}

	constexpr std::size_t kMaxComponents = 32;

	class Component {
	public:
		Component();
		virtual ~Component();

		virtual void Init();
		virtual void Update();
		virtual void Render();

		Entity* parent_entity;
	};

	class Entity {
	public:
		Entity();

		template <typename T, typename... TArgs>
		T& AddComponent(TArgs&&... mArgs) {
			T* component(new T(std::forward<TArgs>(mArgs)...));
			component->parent_entity = this;

			std::unique_ptr<Component> component_pointer{ component };
			components.emplace_back(std::move(component_pointer));

			component_array[GetComponentTypeID<T>()] = component;
			component_set[GetComponentTypeID<T>()] = true;

			component->Init();
			return *component;
		}

		template <typename T>
		T& GetComponent() {
			auto component_pointer(component_array[GetComponentTypeID<T>()]);
			return *static_cast<T*>(component_pointer);
		}

		template<typename T> bool HasComponent() {
			return component_set[GetComponentTypeID<T>];
		}

		void Update();
		void Render();
	private:
		std::vector<std::unique_ptr<Component>> components;
		std::bitset<kMaxComponents> component_set;
		std::array<Component*, kMaxComponents> component_array;
	};
}

#endif // !ENTITY_H
