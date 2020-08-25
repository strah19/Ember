#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <functional>
#include <vector>

namespace ember {
	struct Event {
	public:
		Event(const std::string& name)
			: active(true), Handled(false), name(name) { }
		virtual ~Event() = default;

		bool ActivityCheck() const { return active; }
		void Active(bool active) { this->active = active; }

		bool Handled;

		virtual std::string GetName() const = 0;
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
			if (event->ActivityCheck()) {
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

#endif // !EVENT_H
