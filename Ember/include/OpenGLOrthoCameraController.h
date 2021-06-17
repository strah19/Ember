#ifndef ORTHO_CAMERA_CONTROLLER_H
#define ORTHO_CAMERA_CONTROLLER_H

#include "OpenGLOrthoCamera.h"
#include "WindowEvents.h"
#include "Events.h"
#include "MouseEvents.h"

namespace EmberGL {
	using namespace Ember;

	class OrthoCameraController {
	public:
		OrthoCameraController() = default;
		OrthoCameraController(const glm::vec2& window_size);

		OrthoCamera& GetCamera() { return camera; }
		void OnEvent(Event& event);
		void Update();
		inline void SetFreeze(bool freeze) { this->freeze = freeze; }
	private:
		void MouseWheelHandler(const MouseWheelEvents& mousewheel);
		void ResizeHandler(const ResizeEvent& resize);
	private:
		OrthoCamera camera;

		float aspect_ratio = 0.0f;
		float zoom = -1.0f;
		glm::vec3 camera_pos = { 0, 0, 0 };
		glm::vec2 camera_speed = glm::vec2(0.2f, 0.2f);
		bool freeze = false;
	};
}

#endif // !ORTHO_CAMERA_CONTROLLER_H
