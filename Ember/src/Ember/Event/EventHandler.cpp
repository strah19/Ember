#include "EmberPch.h"
#include "EventHandler.h"

namespace ember {
	EventHandler::EventHandler(Window* window, Input* input)
		: window(nullptr), input(nullptr), native_event_handler() {
		if (window != nullptr && input != nullptr) {
			this->window = window;
			this->input = input;
		}
	}

	void EventHandler::Update() {
		while (SDL_PollEvent(&native_event_handler)) {
			KeyEvent();
			QuitEvent();
			MouseEvent();
			ResizeEvent();
		}
	}

	void EventHandler::QuitEvent() {
		if (native_event_handler.type == SDL_QUIT) {
			ember::QuitEvent quit(true);
			callback(quit);
			window->Quit();
		}
	}

	void EventHandler::KeyEvent() {
		if (native_event_handler.type == SDL_KEYDOWN) {
			KeyboardEvents keyboard(true, native_event_handler.key.repeat, native_event_handler.key.keysym.scancode);
			callback(keyboard);
		}
	}

	void EventHandler::MouseEvent() {
		if (native_event_handler.type == SDL_MOUSEBUTTONDOWN) {
			ember::MouseButtonEvents mouse(true, native_event_handler.button.button, native_event_handler.button.clicks);
			input->mouse_event = mouse;
			callback(mouse);
		}
		else if (native_event_handler.type == SDL_MOUSEBUTTONUP) {
			ember::MouseButtonEvents mouse(false, native_event_handler.button.button, native_event_handler.button.clicks);
			input->mouse_event = mouse;
			callback(mouse);
		}
	}

	void EventHandler::ResizeEvent() {
		if (native_event_handler.type == SDL_WINDOWEVENT) {
			if (native_event_handler.window.event == SDL_WINDOWEVENT_RESIZED) {
				ember::ResizeEvent resize(native_event_handler.window.data1, native_event_handler.window.data2);
				callback(resize);
			}
		}
		SDL_SetWindowResizable(window->GetNativeWindow(), SDL_TRUE);
	}

	void EventHandler::MousePositonEvent() {
		if (native_event_handler.type == SDL_MOUSEMOTION) {
			ember::MouseButtonPositionEvents pos(native_event_handler.button.x, native_event_handler.button.y);
			input->mouse_pos = pos;
			callback(pos);
		}
	}
}