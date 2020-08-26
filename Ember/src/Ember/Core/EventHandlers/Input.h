#ifndef INPUT_H
#define INPUT_H

#include "EventHandler.h"

namespace ember {
	class Input {
	public:
		Input(EventHandler* handler)
			: handler(handler) { }

		bool KeyPressed() { return handler->Event().keyboard.pressed; }
		std::string PressedKey() { return handler->Event().keyboard.name; }
		IVec2 MousePosition() { return handler->Event().mouse.position; }
		bool Down() { return handler->Event().mouse.down; }
		int ButtonId() { return handler->Event().mouse.button_id; }
		int ButtonClicks() { return handler->Event().mouse.clicks; }
	private:
		EventHandler* handler;
	};
}

#endif // !INPUT_H
