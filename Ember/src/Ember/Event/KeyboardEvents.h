#ifndef KEYBOARD_EVENTS_H
#define KEYBOARD_EVENTS_H

#include "Event.h"

namespace ember {
	struct KeyboardEvents : public Event {
		KeyboardEvents(const std::string& name)
			: Event(name), pressed(false), repeat(0) { }
		virtual ~KeyboardEvents() = default;
		
		std::string GetName() const { return name; }

		bool pressed;
		int repeat;
		std::string key_name;

		std::string text_input;
	};
}

#endif // !KEYBOARD_EVENTS_H
