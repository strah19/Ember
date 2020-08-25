#ifndef WINDOW_EVENTS_H
#define WINDOW_EVENTS_H

#include "Event.h"

namespace ember {
	struct WindowEvents : public Event {
		WindowEvents(const std::string& name)
			: Event(name), closed(false) { }
		virtual ~WindowEvents() = default;
		
		std::string GetName() const { return name; }

		bool closed;
	};

	struct ResizeEvent : public Event {
		ResizeEvent(const std::string& name)
			: Event(name), resize(false) { }
		virtual ~ResizeEvent() = default;

		std::string GetName() const { return name; }

		bool resize;
	};
}

#endif // !WINDOW_EVENTS_H
