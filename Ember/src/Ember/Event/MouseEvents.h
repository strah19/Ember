#ifndef MOUSE_EVENTS_H
#define MOUSE_EVENTS_H

#include "Event.h"
#include "Structures/Vec2.h"

namespace ember {
	struct MouseButtonEvents : public Event {
		MouseButtonEvents(bool down, int id, int clicks)
			: Event("Mouse"), down(down), button_id(id), clicks(clicks) { }
		virtual ~MouseButtonEvents() = default;

		std::string GetName() const { return name; }

		bool down;
		int button_id;
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