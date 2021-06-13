#include "EventHandler.h"

namespace Ember {
	static MouseButtonEvents mouse_event = MouseButtonEvents(false, 0, 0, false);
	static KeyboardEvents keyboard_event = KeyboardEvents(false, 0, EmberKeyCode::Null);
	static MouseButtonPositionEvents mouse_pos = MouseButtonPositionEvents({ 0, 0 }, { 0, 0 });
	static KeyboardTextInputEvents input = KeyboardTextInputEvents(" ");
	static MouseWheelEvents wheel = MouseWheelEvents(0);

	bool Events::Down() { return mouse_event.down; }
	IVec2 Events::MousePosition() { return mouse_pos.position; }
	ButtonIds Events::ButtonId() { return mouse_event.button_id; }
	bool Events::Clicked() { return mouse_event.clicked; }

	EmberKeyCode Events::KeyCode() { return keyboard_event.scancode; }
	bool Events::KeyDown() { return keyboard_event.pressed; }

	IVec2 Events::MouseMotion() { return mouse_pos.motion; }
	int Events::MouseWheelDirection() { return wheel.direction; }
	void Events::ResetWheel() { wheel.direction = 0; }

	std::string Events::UserInput() { return input.input_text; }

	EventHandler::EventHandler(Window* window)
		: window(nullptr), native_event_handler() {
		if (window != nullptr)
			this->window = window;
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
			KeyboardEvents keyboard(true, native_event_handler.key.repeat, (EmberKeyCode)native_event_handler.key.keysym.scancode);
			keyboard_event = keyboard;
			callback(keyboard);
		}
		else if (native_event_handler.type == SDL_KEYUP) {
			KeyboardEvents keyboard(false, 0, EmberKeyCode::Null);
			keyboard_event = keyboard;
			callback(keyboard);
		}
	}

	void EventHandler::KeyboardInputEvent() {
		if (native_event_handler.type == SDL_TEXTINPUT) {
			KeyboardTextInputEvents input(native_event_handler.text.text);
			input = input;
			callback(input);
		}
	}

	void EventHandler::MouseEvent() {
		if (native_event_handler.type == SDL_MOUSEBUTTONDOWN) {
			MouseButtonEvents mouse(true, native_event_handler.button.button, native_event_handler.button.clicks, true);
			mouse_event = mouse;
			callback(mouse);
		}
		else if (native_event_handler.type == SDL_MOUSEBUTTONUP) {
			MouseButtonEvents mouse(false, native_event_handler.button.button, native_event_handler.button.clicks, false);
			mouse_event = mouse;
			callback(mouse);
		}
		else
			mouse_event.clicked = false;
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
			MouseButtonPositionEvents pos({ native_event_handler.button.x, native_event_handler.button.y }, { native_event_handler.motion.xrel, native_event_handler.motion.yrel });
			mouse_pos = pos;
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
			MouseWheelEvents wheel(native_event_handler.wheel.y);
			wheel = wheel;
			callback(wheel);
		}
	}
}