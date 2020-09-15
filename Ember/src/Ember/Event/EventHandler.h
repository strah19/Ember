#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "Event/MouseEvents.h"
#include "Event/WindowEvents.h"
#include "Event/KeyboardEvents.h"
#include "Platform/Window/WindowsWindow.h"

namespace ember {
	using Callback = std::function<void(ember::Event&)>;

	class Input {
	public:
		Input() 
			: mouse_event(false, 0, 0), keyboard_event(false, 0, 0), mouse_pos(0, 0) {

		}

		bool Down() const { return mouse_event.down; }
		IVec2 MousePosition() const { return mouse_pos.position; }
		int ButtonId() const { return mouse_event.button_id; }
	private:
		ember::MouseButtonEvents mouse_event;
		ember::KeyboardEvents keyboard_event;
		ember::MouseButtonPositionEvents mouse_pos;
		friend class EventHandler;
	};

	class EventHandler {
	public:
		EventHandler(Window* window, Input* input);

		void Update();
		void SetEventCallback(const Callback& callback) { this->callback = callback; }

		SDL_Event NativeEvent() const { return native_event_handler; }
	private:
		Window* window;
		SDL_Event native_event_handler;
		Callback callback;
		Input* input;

		void QuitEvent();
		void KeyEvent();
		void ResizeEvent();
		void MouseEvent();
		void MousePositonEvent();

	};
}

#endif // !EVENT_HANDLER_H