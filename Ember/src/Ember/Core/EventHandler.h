#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "Core/Events/Events.h"
#include "Core/Events/MouseEvents.h"
#include "Core/Events/WindowEvents.h"
#include "Core/Events/KeyBoardEvents.h"
#include "Core/Events/JoystickEvents.h"

#include "Core/Window.h"

namespace Ember {
	using Callback = std::function<void(Event&)>;

	class Events {
	public:
		Events()
			: mouse_event(false, 0, 0, false), keyboard_event(false, 0, EmberKeyCode::Null), mouse_pos({ 0, 0 }, { 0, 0 }), input(" "), wheel(0) { }

		bool Down() const { return mouse_event.down; }
		IVec2 MousePosition() const { return mouse_pos.position; }
		ButtonIds ButtonId() const { return mouse_event.button_id; }
		EmberKeyCode KeyCode() const { return keyboard_event.scancode; }
		bool KeyDown() const { return keyboard_event.pressed; }
		IVec2 MouseMotion() const { return mouse_pos.motion; }
		std::string UserInput() const { return input.input_text; }
		int MouseWheelDirection() const { return wheel.direction; }
		void ResetWheel() { wheel.direction = 0; }
		bool Clicked() { return mouse_event.clicked; }
	private:
		MouseButtonEvents mouse_event;
		KeyboardEvents keyboard_event;
		MouseButtonPositionEvents mouse_pos;
		KeyboardTextInputEvents input;
		MouseWheelEvents wheel;
		friend class EventHandler;
	};

	class EventHandler {
	public:
		EventHandler(Window* window, Events* events);

		void Update();
		void SetEventCallback(const Callback& callback) { this->callback = callback; }

		SDL_Event* NativeEvent() { return &native_event_handler; }
	private:
		Window* window;
		SDL_Event native_event_handler;
		Callback callback;
		Events* events;

		void Quit();
		void KeyEvent();
		void WindowResizeEvent();
		void MouseEvent();
		void MousePositonEvent();
		void KeyboardInputEvent();
		void JoystickEvent();
		void MouseWheelEvent();
	};
}

#endif // !EVENT_HANDLER_H
