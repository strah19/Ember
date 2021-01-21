#ifndef MOUSE_EVENTS_H
#define MOUSE_EVENTS_H

#include "Events.h"

namespace Ember {
	enum class ButtonIds {
		LeftMouseButton = 1, MiddleMouseButton = 2, RightMouseButton = 3
	};

	struct MouseButtonEvents : public Event {
		MouseButtonEvents(bool down, int id, int clicks, bool clicked)
			: Event("Mouse"), down(down), button_id(static_cast<ButtonIds>(id)), clicks(clicks), clicked(clicked) { }
		virtual ~MouseButtonEvents() = default;

		std::string GetName() const { return name; }

		bool down;
		ButtonIds button_id;
		int clicks;
		bool clicked;
	};

	struct MouseButtonPositionEvents : public Event {
		MouseButtonPositionEvents(const Ember::IVec2& pos, const Ember::IVec2& motion)
			: Event("Position"), position(pos), motion(motion) { }
		virtual ~MouseButtonPositionEvents() = default;

		std::string GetName() const { return name; }

		IVec2 position;
		IVec2 motion;
	};

	struct MouseWheelEvents : public Event {
		MouseWheelEvents(int direction)
			: Event("Wheel"), direction(direction) { }
		virtual ~MouseWheelEvents() = default;

		std::string GetName() const { return name; }

		int direction;
	};
}

#endif // !MOUSE_EVENTS_H
