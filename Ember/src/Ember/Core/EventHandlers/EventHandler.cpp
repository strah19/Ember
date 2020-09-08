#include "EmberPch.h"
#include "EventHandler.h"

namespace ember {
	EventHandler::EventHandler(Window* window)	
		: window(nullptr), native_event_handler(), is_resize_active(false) {
		if (window != nullptr) {
			this->window = window;
		}
		InitializeEvents();
	}

	void EventHandler::Update()	{
		while (SDL_PollEvent(&native_event_handler)) {
			KeyEvents();
			WinEvents();
			Mouse();

			if(is_resize_active)
				Resize();
		}
	}

	bool EventHandler::WinEvents() {
		return dispatchers[GetEventType<WindowEvents>()].Dispatch<WindowEvents>([&](WindowEvents& window_event) {
			if (native_event_handler.type == SDL_QUIT) {
				window->Quit();
				window_event.closed = true;
				return true;
			}
			if (native_event_handler.type == SDL_WINDOWEVENT) {
				if (native_event_handler.window.event == SDL_WINDOWEVENT_RESIZED) {
					window->Properties()->width = native_event_handler.window.data1;
					window->Properties()->height = native_event_handler.window.data2;
					return true;
				}
				if (native_event_handler.window.event == SDL_WINDOWEVENT_MOVED) {
					window->Properties()->position.x = native_event_handler.window.data1;
					window->Properties()->position.y = native_event_handler.window.data2;
					return true;
				}
			}
			return false;
		});
	}

	bool EventHandler::KeyEvents() {
		return dispatchers[GetEventType<KeyboardEvents>()].Dispatch<KeyboardEvents>([&](KeyboardEvents& keyboard_event) {
			if (native_event_handler.type == SDL_KEYDOWN) {
				keyboard_event.pressed = true;
				keyboard_event.repeat = native_event_handler.key.repeat;
				keyboard_event.key_name = SDL_GetKeyName(native_event_handler.key.keysym.sym);
				return true;
			}
			else if(native_event_handler.type == SDL_KEYUP)
				keyboard_event.pressed = false;
			else if (native_event_handler.type == SDL_TEXTINPUT) {
				keyboard_event.text_input = native_event_handler.text.text;
			}
			return false;
		});
	}

	bool EventHandler::Mouse() {
		return dispatchers[GetEventType<MouseButtonEvents>()].Dispatch<MouseButtonEvents>([&](MouseButtonEvents& mouse_event) {
			if (native_event_handler.type == SDL_MOUSEBUTTONDOWN) {
				mouse_event.down = true;
				mouse_event.button_id = native_event_handler.button.button;
				mouse_event.clicks = native_event_handler.button.clicks;
				return true;
			}
			else if (native_event_handler.type == SDL_MOUSEBUTTONUP) {
				mouse_event.down = false;
				return true;
			}
			else if (native_event_handler.type == SDL_MOUSEWHEEL) {
				mouse_event.vert = native_event_handler.wheel.y;
				mouse_event.horz = native_event_handler.wheel.x;
				return true;
			}

			mouse_event.position = { native_event_handler.button.x, native_event_handler.button.y };

			return false;
		});
	}

	bool EventHandler::Resize() {
		return dispatchers[GetEventType<ResizeEvent>()].Dispatch<ResizeEvent>([&](ResizeEvent& resize_event) {
			SDL_SetWindowResizable((window->GetNativeWindow()), SDL_TRUE);
			int w, h;
			SDL_GetWindowSize((window->GetNativeWindow()), &w, &h);
			window->Properties()->width = w;
			window->Properties()->height = h;

			return false;
		});
	}

	void EventHandler::InitializeEvents() {
		GetEventType<MouseButtonEvents>();
		dispatchers.push_back(EventDispatcher(&events.mouse));		

		GetEventType<KeyboardEvents>();
		dispatchers.push_back(EventDispatcher(&events.keyboard));

		GetEventType<WindowEvents>();
		dispatchers.push_back(EventDispatcher(&events.window));

		GetEventType<ResizeEvent>();
		dispatchers.push_back(EventDispatcher(&events.resize));
	}
}