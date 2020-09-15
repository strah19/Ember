#ifndef KEYBOARD_EVENTS_H
#define KEYBOARD_EVENTS_H

#include "Event.h"

namespace ember {
	struct KeyboardEvents : public Event {
		KeyboardEvents(bool pressed, int repeat, int scancode)
			: Event("Keyboard"), pressed(pressed), repeat(repeat), scancode(scancode) { }
		virtual ~KeyboardEvents() = default;

		std::string GetName() const { return name; }

		bool pressed;
		int repeat;
		int scancode;
	};
}

#endif // !KEYBOARD_EVENTS_H