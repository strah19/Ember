#ifndef MOUSE_EVENTS_H
#define MOUSE_EVENTS_H

#include "Events.h"

namespace Ember {
	enum class ButtonIds {
		LeftMouseButton = 1, MiddleMouseButton = 2, RightMouseButton = 3
	};

	struct MouseButtonEvents : public Event {
		MouseButtonEvents(bool down, int id, int clicks)
			: Event("Mouse"), down(down), button_id(static_cast<ButtonIds>(id)), clicks(clicks) { }
		virtual ~MouseButtonEvents() = default;

		std::string GetName() const { return name; }

		bool down;
		ButtonIds button_id;
		int clicks;
	};

	struct MouseButtonPositionEvents : public Event {
		MouseButtonPositionEvents(int x, int y)
			: Event("Position"), position(x, y) { }
		virtual ~MouseButtonPositionEvents() = default;

		std::string GetName() const { return name; }

		IVec2 position;
	};

}

#endif // !MOUSE_EVENTS_H
