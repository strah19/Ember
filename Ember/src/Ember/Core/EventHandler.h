#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "Event/MouseEvents.h"
#include "Event/WindowEvents.h"
#include "Event/KeyboardEvents.h"
#include "Platform/Window/WindowsWindow.h"
#include <vector>
#include <unordered_map>

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

	class EventHandler {
	public:
		EventHandler(Window* window);
		~EventHandler();

		void Update();
		bool KeyPressed() { return static_cast<KeyboardEvents*>(dispatchers[GetEventType<KeyboardEvents>()].GetEvent())->pressed; }
		std::string PressedKey() { return static_cast<KeyboardEvents*>(dispatchers[GetEventType<KeyboardEvents>()].GetEvent())->name; }
		void ResizeWin() { is_resize_active = true; }

		template <typename T>
		void AddEvent(const std::string& name) {
			GetEventType<T>();
			events.push_back(new T(name));
			dispatchers.push_back(EventDispatcher(events.back()));
		}
	private:
		Window* window;
		SDL_Event native_event_handler;

		std::vector<EventDispatcher> dispatchers;
		std::vector<Event*> events;

		bool is_resize_active;

		void InitializeEvents();

		bool WinEvents();
		bool KeyEvents();
		bool Resize();
	};
}

#endif // !EVENT_HANDLER_H
