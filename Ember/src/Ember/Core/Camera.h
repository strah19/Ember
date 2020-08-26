#ifndef CAMERA_H
#define CAMERA_H

#include "../Structures/Vec2.h"
#include "EventHandlers/Input.h"

namespace ember {
	class Camera {
	public:
		Camera();
		Camera(float offset_x, float offset_y);
		void Pan(Input& input, int button_click);
		void Scale(float scale_x, float scale_y);
		void MoveCameraOffset(float offset_x, float offset_y);

		void WorldToScreen(float f_world_x, float f_world_y, int& screen_x, int& screen_y);
		void ScreenToWorld(int screen_x, int screen_y, float& f_world_x, float& f_world_y);

		Vec2 Offset() { return { offset_x, offset_y }; }
		Vec2 ScaleValue() { return { scale_x, scale_y }; }
	private:
		float offset_x, offset_y;
		float scale_x, scale_y;
		bool down_flag;
		float pan_x, pan_y;
	};
}


#endif // !CAMERA_H

