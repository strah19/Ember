#include "GuiObjectFramework.h"

namespace Ember {
	Scaleable::Scaleable(const Rect& position, const IVec2& window_size) : percents({ 0, 0, 0, 0 }) {
		CalculatePercents(position, window_size);
	}

	Scaleable::Scaleable() : percents({ 0, 0, 0, 0 }) {
	}

	void Scaleable::CalculatePercents(const Rect& position, const IVec2& window_size) {
		percents.x = (float) position.x / window_size.x * 100;
		percents.y = (float) position.y / window_size.y * 100;
		percents.w = (float) position.w / window_size.x * 100;
		percents.h = (float) position.h / window_size.y * 100;
	}

	Rect Scaleable::Update(const IVec2& window_size) {
		return (Ember::Rect({ (int) ((percents.x / 100) * window_size.x), (int) ((percents.y / 100) * window_size.y),
				(int) ((percents.w / 100) * window_size.x), (int) ((percents.h / 100) * window_size.y) }));
	}

	void Dragable::CalculateNewPosition(Rect& position, Events* events, ButtonIds button_clicked) {
		if (events->Down() && events->ButtonId() == button_clicked && events != nullptr) {
			if (!is_moving) {
				is_moving = true;
				pre_x = events->MousePosition().x - position.x;
				pre_y = events->MousePosition().y - position.y;
			}
		}
		else if (is_moving) {
			position.pos = Ember::IVec2(events->MousePosition().x - pre_x, events->MousePosition().y - pre_y);
		}
		if (!events->Down())
			is_moving = false;
	}
}