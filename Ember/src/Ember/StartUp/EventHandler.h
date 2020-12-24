#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "StartUp/Events/Events.h"
#include "StartUp/Events/MouseEvents.h"
#include "StartUp/Events/WindowEvents.h"
#include "StartUp/Events/KeyBoardEvents.h"
#include "StartUp/Events/JoystickEvents.h"

#include "StartUp/Window.h"

namespace Ember {
	using Callback = std::function<void(Event&)>;

	class Events {
	public:
		Events()
			: mouse_event(false, 0, 0), keyboard_event(false, 0, EmberKeyCode::Null), mouse_pos({ 0, 0 }, { 0, 0 }) { }

		bool Down() const { return mouse_event.down; }
		IVec2 MousePosition() const { return mouse_pos.position; }
		ButtonIds ButtonId() const { return mouse_event.button_id; }
		EmberKeyCode KeyCode() { return keyboard_event.scancode; }
		bool KeyDown() { return keyboard_event.pressed; }
	private:
		MouseButtonEvents mouse_event;
		KeyboardEvents keyboard_event;
		MouseButtonPositionEvents mouse_pos;
		friend class EventHandler;
	};

	class EventHandler {
	public:
		EventHandler(Window* window, Events* events);

		void Update();
		void SetEventCallback(const Callback& callback) { this->callback = callback; }

		SDL_Event NativeEvent() const { return native_event_handler; }
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
