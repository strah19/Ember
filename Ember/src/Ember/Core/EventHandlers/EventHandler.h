#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "Event/MouseEvents.h"
#include "Event/WindowEvents.h"
#include "Event/KeyboardEvents.h"
#include "Platform/Window/WindowsWindow.h"

namespace ember {
	using EventType = std::size_t;

	inline EventType GetEventType() {
		static EventType current_component_id = 0;
		return current_component_id++;
	}

	template <typename T> inline EventType GetEventType() {
		static EventType typeID = GetEventType();
		return typeID;
	}

	struct Events {
		WindowEvents window;
		MouseButtonEvents mouse;
		KeyboardEvents keyboard;
		ResizeEvent resize;

		Events()
			: window("Window"), mouse("Mouse"), keyboard("Keyboard"), resize("Resize") { }
	};

	class EventHandler {
	public:
		EventHandler(Window* window);

		void Update();
		void ResizeWin() { is_resize_active = true; }

		Events Event() { return events; }
		SDL_Event NativeEvent() const { return native_event_handler; }
	private:
		Window* window;
		SDL_Event native_event_handler;
		std::vector<EventDispatcher> dispatchers;
		Events events;
		bool is_resize_active;

		void InitializeEvents();

		bool WinEvents();
		bool KeyEvents();
		bool Resize();
		bool Mouse();
	};
}

#endif // !EVENT_HANDLER_H