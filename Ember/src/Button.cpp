#include "Button.h"

namespace Ember {
	Button::Button(Events* events, const Rect& button) : position({ 0, 0, 0, 0 }) {
		Initialize(events, button);
	}
	
	Button::Button() : events(nullptr), position({ 0, 0, 0, 0 }), in(false), clicked(false) { }

	void Button::Initialize(Events* events, const Rect& button) {
		in = false;
		clicked = false;

		if (events != nullptr) {
			this->events = events;
			position = button;
		}
	}

	bool Button::Hover() {
		if (events->MousePosition().x >= position.x && events->MousePosition().x <= position.x + position.w && events->MousePosition().y >= position.y
			&& events->MousePosition().y <= position.y + position.h) {
			return true;
		}
		return false;
	}

	bool Button::Click(ButtonIds id) {
		if (events->Down() && events->ButtonId() == id && !clicked && Hover()) {
			clicked = true;
			return true;
		}
		if (clicked) {
			if (!events->Down())
				clicked = false;
		}
		return false;
	}

	void Button::UpdatePosition(int x, int y) {
		position.x = x;
		position.y = y;
	}

	void Button::UpdateSize(int w, int h) {
		position.w = w;
		position.h = h;
	}

	bool Button::InOut(ButtonIds id) {
		bool click = Click(id);
		if (click && !in)
			in = true;
		else if (click && in)
			in = false;

		return in;
	}

	bool Button::Hold(ButtonIds id) {
		if (events->Down() && events->ButtonId() == id)
			if (events->MousePosition().x >= position.x && events->MousePosition().x <= position.x + position.w
				&& events->MousePosition().y >= position.y && events->MousePosition().y <= position.y + position.h)
				return true;
		return false;
	}
}