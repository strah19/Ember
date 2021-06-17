#ifndef PERSPECTIVE_CAMERA_CONTROLLER_H
#define PERSPECTIVE_CAMERA_CONTROLLER_H

#include "OpenGLPerspectiveCamera.h"
#include "Events.h"
#include "MouseEvents.h"
#include "WindowEvents.h"
#include "KeyboardEvents.h"
#include "Cursor.h"

namespace EmberGL {
	using namespace Ember;
	
	class PerspectiveCameraController {
	public:
		PerspectiveCameraController() = default;
		PerspectiveCameraController(glm::vec2& window_size);
		void OnEvent(Event& event);
		void Update();
		void SetFreeze(bool freeze);
		inline void SetCameraSpeed(float speed) { this->speed = speed; }

		inline glm::vec3 GetFrontCamera() const { return camera_front; }
		inline PerspectiveCamera& GetCamera() { return camera; }
	private:
		void MouseWheelHandler(MouseWheelEvents& mousewheel);
		void KeyboardHandler(KeyboardEvents& keyboard);
		void WindowResizeHandler(ResizeEvent& resize);
	private:
		float aspect_ratio = 0.0f;
		PerspectiveCamera camera;
		float fov = 60.0f;

		glm::vec3 camera_pos = glm::vec3(0, 0, 3 );
		glm::vec3 camera_target = glm::vec3(0, 0, 0);
		glm::vec3 camera_direction = glm::normalize(camera_pos - camera_target);

		float yaw = -90.0f, pitch = 0.0f;
		glm::vec2 last_mouse_position = { 0, 0 };
		glm::vec3 latest_camera_position = { 0, 0, 0 };

		glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
		float speed = 0.2f;

		bool in_camera_mode = false;
		bool freeze = false;

		Cursor cursor_handler;
	};
}

#endif // !PERSPECTIVE_CAMERA_CONTROLLER_H
