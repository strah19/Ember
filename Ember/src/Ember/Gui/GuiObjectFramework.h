#ifndef GUI_OBJECT_FRAMEWORK_H
#define GUI_OBJECT_FRAMEWORK_H

#include "Ember.h"
#include "StartUp/EventHandler.h"

namespace Ember {
	class Scaleable {
	public:
		Scaleable(const Rect& position, const IVec2& window_size);
		Scaleable();

		void CalculatePercents(const Rect& position, const IVec2& window_size);
		Rect Update(const IVec2& window_size);
	private:
		FRect percents;
	};

	class Dragable {
	public:
		void CalculateNewPosition(Rect& position, Events* events, ButtonIds button_clicked);
	private:
		bool is_moving = false;
		int pre_x = 0, pre_y = 0;
	};
}

#endif // !GUI_OBJECT_FRAMEWORK_H
