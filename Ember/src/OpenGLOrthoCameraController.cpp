#include "OpenGLOrthoCameraController.h"
#include "KeyboardCodes.h"
#include "EventHandler.h"

namespace EmberGL {
	using namespace Ember;

	constexpr float ZOOM_SPEED = 3.0f;
	OrthoCameraController::OrthoCameraController(const glm::vec2& window_size) {
		aspect_ratio = (float) window_size.x / (float) window_size.y;
		camera = OrthoCamera(aspect_ratio * zoom, -aspect_ratio * zoom, zoom, -zoom);
	}

	void OrthoCameraController::Update() {
		if (!freeze) {
			camera.SetPosition(camera_pos);
			if (KeyboardEvents::GetKeyboardState(EmberKeyCode::D))
				camera_pos.x += camera_speed.x;
			if (KeyboardEvents::GetKeyboardState(EmberKeyCode::A))
				camera_pos.x -= camera_speed.x;
			if (KeyboardEvents::GetKeyboardState(EmberKeyCode::W))
				camera_pos.y += camera_speed.y;
			if (KeyboardEvents::GetKeyboardState(EmberKeyCode::S))
				camera_pos.y -= camera_speed.y;
		}
	}

	void OrthoCameraController::OnEvent(Event& event) {
		EventDispatcher dispatcher(&event);
		if (!freeze) 
			dispatcher.Dispatch<MouseWheelEvents>(EMBER_BIND_FUNC(MouseWheelHandler));
		dispatcher.Dispatch<ResizeEvent>(EMBER_BIND_FUNC(ResizeHandler));
	}

	void OrthoCameraController::MouseWheelHandler(const MouseWheelEvents& mousewheel) {
		zoom += mousewheel.direction * ZOOM_SPEED;
		camera.SetProjection(aspect_ratio * zoom, -aspect_ratio * zoom, zoom, -zoom);
	}

	void OrthoCameraController::ResizeHandler(const ResizeEvent& resize) {
		aspect_ratio = (float) resize.w / (float) resize.h;
		camera.SetProjection(aspect_ratio * zoom, -aspect_ratio * zoom, zoom, -zoom);
	}
}