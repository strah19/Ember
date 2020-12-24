#ifndef KEYBOARD_EVENTS_H
#define KEYBOARD_EVENTS_H

#include "Events.h"
#include "KeyboardCodes.h"

namespace Ember {
	struct KeyboardEvents : public Event {
		KeyboardEvents(bool pressed, int repeat, EmberKeyCode scancode)
			: Event("Keyboard"), pressed(pressed), repeat(repeat), scancode(scancode) { }
		virtual ~KeyboardEvents() = default;

		std::string GetName() const { return name; }

		bool pressed;
		int repeat;
		EmberKeyCode scancode;
	};

	struct KeyboardTextInputEvents : public Event{
		KeyboardTextInputEvents(const std::string& input)
			: Event("KeyboardTextInput"), input_text(input) { }
		virtual ~KeyboardTextInputEvents() = default;

		static inline void StartTextInput() { SDL_StartTextInput(); }
		static inline void StopTextInput() { SDL_StopTextInput(); }

		std::string input_text;
	};
}

#endif // !KEYBOARD_EVENTS_H
