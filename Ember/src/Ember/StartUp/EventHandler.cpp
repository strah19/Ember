#include "EventHandler.h"

namespace Ember {
	EventHandler::EventHandler(Window* window, Events* events)
		: window(nullptr), events(nullptr), native_event_handler() {
		if (window != nullptr && events != nullptr) {
			this->window = window;
			this->events = events;
		}
	}

	void EventHandler::Update() {
		while (SDL_PollEvent(&native_event_handler)) {
			KeyEvent();
			Quit();
			MouseEvent();
			MousePositonEvent();
			Resize();
		}
	}

	void EventHandler::Quit() {
		if (native_event_handler.type == SDL_QUIT) {
			QuitEvent quit(true);
			callback(quit);
			window->Quit();
		}
	}

	void EventHandler::KeyEvent() {
		if (native_event_handler.type == SDL_KEYDOWN) {
			KeyboardEvents keyboard(true, native_event_handler.key.repeat, native_event_handler.key.keysym.scancode);
			events->keyboard_event = keyboard;
			callback(keyboard);
		}
		else if (native_event_handler.type == SDL_KEYUP) {
			KeyboardEvents keyboard(false, 0, 0);
			events->keyboard_event = keyboard;
			callback(keyboard);
		}
	}

	void EventHandler::MouseEvent() {
		if (native_event_handler.type == SDL_MOUSEBUTTONDOWN) {
			MouseButtonEvents mouse(true, native_event_handler.button.button, native_event_handler.button.clicks);
			events->mouse_event = mouse;
			callback(mouse);
		}
		else if (native_event_handler.type == SDL_MOUSEBUTTONUP) {
			MouseButtonEvents mouse(false, native_event_handler.button.button, native_event_handler.button.clicks);
			events->mouse_event = mouse;
			callback(mouse);
		}
	}

	void EventHandler::Resize() {
		if (native_event_handler.type == SDL_WINDOWEVENT) {
			if (native_event_handler.window.event == SDL_WINDOWEVENT_RESIZED) {
				ResizeEvent resize(native_event_handler.window.data1, native_event_handler.window.data2);
				window->Properties()->width = resize.w;
				window->Properties()->height = resize.h;
				callback(resize);
			}
		}
		SDL_SetWindowResizable(window->GetNativeWindow(), SDL_TRUE);
	}

	void EventHandler::MousePositonEvent() {
		if (native_event_handler.type == SDL_MOUSEMOTION) {
			MouseButtonPositionEvents pos(native_event_handler.button.x, native_event_handler.button.y);
			events->mouse_pos = pos;
			callback(pos);
		}
	}

}