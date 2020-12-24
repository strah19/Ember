#ifndef JOYSTICK_EVENTS_H
#define JOYSTICK_EVENTS_H

#include "Events.h"
#include "Ember.h"

namespace Ember {
	static SDL_Joystick* joystick = nullptr;

	struct JoystickEvents : public Event {
		JoystickEvents(int axis, int value, int button) :
			Event("Joystick"), axis(axis), value(value), button(button) { }

		static inline void SetUpJoystick() { 
			if (SDL_NumJoysticks() < 1) 
				std::cout << "Error: No joystick connected.\n";
			else {
				joystick = SDL_JoystickOpen(0);
				if (joystick == nullptr) 
					std::cout << "Error: Joystick object is nullptr.\n";
			}
		}
		static inline void DestroyJoystick() {
			SDL_JoystickClose(joystick);
			joystick = nullptr;
		}

		int axis;
		int value;
		int button;
	};
}

#endif // !JOYSTICK_EVENTS_H
