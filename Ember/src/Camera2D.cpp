#include "Camera2D.h"

namespace Ember {
	void Camera2D::WorldToScreen(float world_x, float world_y, int& screen_x, int& screen_y) {
		screen_x = (int)((world_x - offset_x) * scale_x);
		screen_y = (int)((world_y - offset_y) * scale_y);
	}

	void Camera2D::ScreenToWorld(int screen_x, int screen_y, float& world_x, float& world_y) {
		world_x = ((float)screen_x / scale_x) + offset_x;
		world_y = ((float)screen_y / scale_y) + offset_y;
	}

	void Camera2D::Pan(Ember::ButtonIds button_id, Ember::Events* events) {
		float mouse_x = (float)events->MousePosition().x;
		float mouse_y = (float)events->MousePosition().y;

		if (events->Clicked() && events->ButtonId() == Ember::ButtonIds::MiddleMouseButton) {
			start_pan_x = mouse_x;
			start_pan_y = mouse_y;
		}

		if (events->Down() && events->ButtonId() == Ember::ButtonIds::MiddleMouseButton) {
			offset_x -= (mouse_x - start_pan_x) / scale_x;
			offset_y -= (mouse_y - start_pan_y) / scale_y;

			start_pan_x = mouse_x;
			start_pan_y = mouse_y;
		}
	}

	void Camera2D::Scale(Ember::Events* events, const Ember::Vec2& scalar) {
		float mouse_x = (float)events->MousePosition().x;
		float mouse_y = (float)events->MousePosition().y;

		float before_zoom_x, before_zoom_y;
		ScreenToWorld((int)mouse_x, (int)mouse_y, before_zoom_x, before_zoom_y);

		scale_x *= scalar.x;
		scale_y *= scalar.y;

		float after_zoom_x, after_zoom_y;
		ScreenToWorld((int)mouse_x, (int)mouse_y, after_zoom_x, after_zoom_y);
		offset_x += (before_zoom_x - after_zoom_x);
		offset_y += (before_zoom_y - after_zoom_y);
	}
}