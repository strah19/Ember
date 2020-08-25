#ifndef MOUSE_EVENTS_H
#define MOUSE_EVENTS_H

#include "Event.h"
#include "Structures/Vec2.h"

namespace ember {
	struct MouseButtonEvents : public Event {
		MouseButtonEvents(const std::string& name) 
			: Event(name), position(0, 0), down(false), button_id(0), clicks(0) { }
		virtual ~MouseButtonEvents() = default;
		
		std::string GetName() const { return name; }

		IVec2 position;
		bool down;
		int button_id;
		int clicks;
	};
}

#endif // !MOUSE_EVENTS_H
