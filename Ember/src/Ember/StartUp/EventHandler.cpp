#include "EventHandler.h"

namespace Ember {
	EventHandler::EventHandler(Window* window, Events* events)
		: window(nullptr), events(nullptr), native_event_handler() {
		if (window != nullptr && events != nullptr) {
			this->window = window;
			this->events = events;
		}
	}

	void EventHandler::Update() {
		while (SDL_PollEvent(&native_event_handler)) {
			KeyEvent();
			Quit();
			MouseEvent();
			MousePositonEvent();
			WindowResizeEvent();
			KeyboardInputEvent();
			JoystickEvent();
			MouseWheelEvent();
		}
	}

	void EventHandler::Quit() {
		if (native_event_handler.type == SDL_QUIT) {
			QuitEvent quit(true);
			callback(quit);
			window->Quit();
		}
	}

	void EventHandler::KeyEvent() {
		if (native_event_handler.type == SDL_KEYDOWN) {
			KeyboardEvents keyboard(true, native_event_handler.key.repeat, (EmberKeyCode) native_event_handler.key.keysym.scancode);
			events->keyboard_event = keyboard;
			callback(keyboard);
		}
		else if (native_event_handler.type == SDL_KEYUP) {
			KeyboardEvents keyboard(false, 0, EmberKeyCode::Null);
			events->keyboard_event = keyboard;
			callback(keyboard);
		}
	}

	void EventHandler::KeyboardInputEvent() {
		if (native_event_handler.type == SDL_TEXTINPUT) {
			KeyboardTextInputEvents input(native_event_handler.text.text);
			callback(input);
		}
	}

	void EventHandler::MouseEvent() {
		if (native_event_handler.type == SDL_MOUSEBUTTONDOWN) {
			MouseButtonEvents mouse(true, native_event_handler.button.button, native_event_handler.button.clicks);
			events->mouse_event = mouse;
			callback(mouse);
		}
		else if (native_event_handler.type == SDL_MOUSEBUTTONUP) {
			MouseButtonEvents mouse(false, native_event_handler.button.button, native_event_handler.button.clicks);
			events->mouse_event = mouse;
			callback(mouse);
		}
	}

	void EventHandler::WindowResizeEvent() {
		if (native_event_handler.type == SDL_WINDOWEVENT) {
			if (native_event_handler.window.event == SDL_WINDOWEVENT_RESIZED) {
				ResizeEvent resize(native_event_handler.window.data1, native_event_handler.window.data2);
				window->Properties()->width = resize.w;
				window->Properties()->height = resize.h;
				callback(resize);
			}
		}
	}

	void EventHandler::MousePositonEvent() {
		if (native_event_handler.type == SDL_MOUSEMOTION) {
			MouseButtonPositionEvents pos({ native_event_handler.button.x, native_event_handler.button.y }, { native_event_handler.motion.x, native_event_handler.motion.y });
			events->mouse_pos = pos;
			callback(pos);
		}
	}

	void EventHandler::JoystickEvent() {
		int axis = 0, value = 0, button = 0;
		if (native_event_handler.type == SDL_JOYBUTTONDOWN) {
			button = native_event_handler.jbutton.button;

			JoystickEvents joystick(axis, value, button);
			callback(joystick);
		}
		if (native_event_handler.type == SDL_JOYAXISMOTION) {
			axis = native_event_handler.jaxis.axis;
			value = native_event_handler.jaxis.value;

			JoystickEvents joystick(axis, value, button);
			callback(joystick);
		}
	}

	void EventHandler::MouseWheelEvent() {
		if (native_event_handler.type == SDL_MOUSEWHEEL) {
			MouseWheelEvents wheel(native_event_handler.wheel.direction);
			callback(wheel);
		}
	}
}