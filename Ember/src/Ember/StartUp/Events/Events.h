#ifndef EVENTS_H
#define EVENTS_H

#include <string>
#include <functional>

#include "Vector.h"

namespace Ember {
	struct Event {
	public:
		Event(const std::string& name)
			: active(true), Handled(false), name(name) { }
		virtual ~Event() = default;

		bool ActivityCheck() const { return active; }
		void Active(bool active) { this->active = active; }

		bool Handled;

		virtual std::string GetName() const { return ""; }
	protected:
		bool active;
		std::string name;
	};

	class EventDispatcher {
	public:
		EventDispatcher(Event* event)
			: event(event) {
		}

		virtual ~EventDispatcher() {
			event = nullptr;
		}

		template<typename T>
		bool Dispatch(const std::function<bool(T&)> func) {
			if (event->ActivityCheck() && dynamic_cast<T*>(event)) {
				event->Handled = func(static_cast<T&>(*event));
				return true;
			}
			return false;
		}

		inline Event* GetEvent() { return event; }
	private:
		Event* event;
	};
}

#endif // !EVENTS_H
