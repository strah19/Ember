#include "EventHandler.h"
#include <memory>

namespace ember {
	EventHandler::EventHandler(Window* window)	
		: window(nullptr), native_event_handler(), is_resize_active(false) {
		if (window != nullptr) {
			this->window = window;
		}
		InitializeEvents();
	}

	EventHandler::~EventHandler() {
		size_t size = events.size();
		for (size_t i = 0; i < events.size(); i++) {
			delete events[i];
		}
	}

	void EventHandler::Update()	{
		while (SDL_PollEvent(&native_event_handler)) {
			KeyEvents();
			WinEvents();

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
				keyboard_event.name = SDL_GetKeyName(native_event_handler.key.keysym.sym);
				return true;
			}
			else if(native_event_handler.type == SDL_KEYUP)
				keyboard_event.pressed = false;
			return false;
		});
	}

	bool EventHandler::Resize() {
		return dispatchers[GetEventType<ResizeEvent>()].Dispatch<ResizeEvent>([&](ResizeEvent& resize_event) {
			SDL_SetWindowResizable(static_cast<SDL_Window*>(window->GetNativeWindow()), SDL_TRUE);
			int w, h;
			SDL_GetWindowSize(static_cast<SDL_Window*>(window->GetNativeWindow()), &w, &h);
			window->Properties()->width = w;
			window->Properties()->height = h;

			return false;
		});
	}

	void EventHandler::InitializeEvents() {
		AddEvent<WindowEvents>("Window");
		AddEvent<ResizeEvent>("Resize");
		AddEvent<KeyboardEvents>("Keyboard");
	}
}