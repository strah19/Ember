#include "EmberPch.h"
#include "Button.h"

namespace ember {
	Button::Button(std::shared_ptr<Input> events, Rect& button)
		: events(events), position(button), in(false), clicked(false) {
	}

	Button::Button(std::shared_ptr<Input> events, int x, int y, int w, int h)
		: events(events), position({ x, y, w, h }), in(false), clicked(false) {
	}

	bool Button::Hover() {
		if (events->MousePosition().x >= position.x && events->MousePosition().x <= position.x + position.w && events->MousePosition().y >= position.y
			&& events->MousePosition().y <= position.y + position.h) {
			return true;
		}
		return false;
	}

	bool Button::Click(int id) {
		if (events->Down() && events->ButtonId() == id && !clicked) {
			if (events->MousePosition().x >= position.x && events->MousePosition().x <= position.x + position.w && events->MousePosition().y >= position.y
				&& events->MousePosition().y <= position.y + position.h) {
				clicked = true;
				return true;
			}
		}
		if (clicked) {
			if (!events->Down()) {
				clicked = false;
			}
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

	bool Button::InOut(int id) {
		bool click = Click(id);
		if (click && !in) {
			in = true;
		}
		else if (click && in) {
			in = false;
		}

		return in;
	}

	bool Button::Hold(int id) {
		if (events->Down() && events->ButtonId() == id) {
			if (events->MousePosition().x >= position.x && events->MousePosition().x <= position.x + position.w
				&& events->MousePosition().y >= position.y && events->MousePosition().y <= position.y + position.h) {
				return true;
			}
		}
		return false;
	}
}