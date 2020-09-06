#include "EmberPch.h"
#include "Camera.h"

namespace ember {
	Camera::Camera()
		: offset_x(0.0f), offset_y(0.0f), scale_x(1.0f), scale_y(1.0f), down_flag(true),
		pan_x(0.0f), pan_y(0.0f) {
	}

	Camera::Camera(float offset_x, float offset_y)
		: offset_x(offset_x), offset_y(offset_y), scale_x(1.0f), scale_y(1.0f), down_flag(true),
		pan_x(offset_x), pan_y(offset_y) {
	}

	void Camera::Pan(Input& input, int button_click) {
		if (input.Down() && down_flag && input.ButtonId() == button_click) {
			pan_x = (float) input.MousePosition().x;
			pan_y = (float) input.MousePosition().y;
			down_flag = false;
		}
		else if (!input.Down()) {
			down_flag = true;
		}

		if (input.Down() && input.ButtonId() == button_click) {
			offset_x -= ((float) input.MousePosition().x - pan_x) / scale_x;
			offset_y -= ((float) input.MousePosition().y - pan_y) / scale_y;

			pan_x = (float) input.MousePosition().x;
			pan_y = (float) input.MousePosition().y;
		}
	}

	void Camera::Scale(float scale_x, float scale_y) {
		this->scale_x *= scale_x;
		this->scale_y *= scale_y;
	}

	void Camera::MoveCameraOffset(float offset_x, float offset_y) {
		offset_x = -offset_x;
		offset_y = -offset_y;
	}

	void Camera::WorldToScreen(float f_world_x, float f_world_y, int& screen_x, int& screen_y) {
		screen_x = (int)((f_world_x - offset_x) * scale_x);
		screen_y = (int)((f_world_y - offset_y) * scale_y);
	}

	void Camera::ScreenToWorld(int screen_x, int screen_y, float& f_world_x, float& f_world_y) {
		f_world_x = (float)((screen_x / scale_y) + offset_x);
		f_world_y = (float)((screen_y / scale_y) + offset_y);
	}
}