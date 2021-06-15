#ifndef CAMERA_H
#define CAMERA_H

#include "EventHandler.h"
#include "MouseEvents.h"

namespace Ember {
	class Camera2D {
	public:
		void WorldToScreen(float world_x, float world_y, int& screen_x, int& screen_y);
		void ScreenToWorld(int screen_x, int screen_y, float& world_x, float& world_y);

		inline void SetOffset(const Ember::Vec2& offset) { offset_x = offset.x; offset_y = offset.y; }

		void Pan(Ember::ButtonIds button_id, Ember::Events* events);
		void Scale(Ember::Events* events, const Ember::Vec2& scalar);
	private:
		float offset_x = 0.0f;
		float offset_y = 0.0f;
		float scale_x = 1.0f;
		float scale_y = 1.0f;

		float start_pan_x = 0.0f;
		float start_pan_y = 0.0f;
	};
}

#endif // !CAMERA_H
