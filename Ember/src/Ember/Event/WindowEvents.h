#ifndef WINDOW_EVENTS_H
#define WINDOW_EVENTS_H

#include "Event.h"

namespace ember {
	struct QuitEvent : public Event {
		QuitEvent(bool closed)
			: Event("Quit"), closed(closed) { }
		virtual ~QuitEvent() = default;

		std::string GetName() const { return name; }

		bool closed;
	};

	struct ResizeEvent : public Event {
		ResizeEvent(int w, int h)
			: Event("Resize"), w(w), h(h) { }
		virtual ~ResizeEvent() = default;

		std::string GetName() const { return name; }

		int w, h;
	};
}

#endif // !WINDOW_EVENTS_H