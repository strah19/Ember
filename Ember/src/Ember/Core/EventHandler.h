#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "Core/Events/Events.h"
#include "Core/Events/MouseEvents.h"
#include "Core/Events/WindowEvents.h"
#include "Core/Events/KeyBoardEvents.h"
#include "Core/Events/JoystickEvents.h"

#include "Core/Windows/Window.h"

namespace Ember {
	using Callback = std::function<void(Event&)>;

	class Events {
	public:
		static bool Down();
		static IVec2 MousePosition();
		static ButtonIds ButtonId();
		static bool Clicked();

		static EmberKeyCode KeyCode();
		static bool KeyDown();

		static IVec2 MouseMotion();
		static int MouseWheelDirection();
		static void ResetWheel();

		static std::string UserInput();
	private:
		friend class EventHandler;
	};

	class EventHandler {
	public:
		EventHandler(Window* window);

		void Update();
		void SetEventCallback(const Callback& callback) { this->callback = callback; }

		SDL_Event* NativeEvent() { return &native_event_handler; }
	private:
		Window* window;
		SDL_Event native_event_handler;
		Callback callback;

		void Quit();
		void KeyEvent();
		void WindowResizeEvent();
		void MouseEvent();
		void MousePositonEvent();
		void KeyboardInputEvent();
		void JoystickEvent();
		void MouseWheelEvent();
	};
}

#endif // !EVENT_HANDLER_H
