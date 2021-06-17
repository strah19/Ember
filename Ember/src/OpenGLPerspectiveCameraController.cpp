#include "OpenGLPerspectiveCameraController.h"
#include "EventHandler.h"
#include "KeyboardCodes.h"

namespace EmberGL {
	using namespace Ember;
	constexpr float SMALLEST_ZOOM = 1.0f;
	constexpr float HIGHEST_ZOOM = 60.0f;

	PerspectiveCameraController::PerspectiveCameraController(glm::vec2& window_size) {
		aspect_ratio = (float)window_size.x / (float)window_size.y;
		camera = PerspectiveCamera(glm::radians(fov), aspect_ratio);
		camera_pos = camera.GetPosition();
		last_mouse_position = { window_size.x / 2, window_size.y / 2 };
		latest_camera_position = camera_pos;
	}

	void PerspectiveCameraController::Update() {
		if (!freeze) {
			if (latest_camera_position != camera_pos)
				camera.SetPosition(camera_pos);
			camera.SetMatrixView(glm::lookAt(camera_pos, camera_pos + camera_front, camera_up));

			latest_camera_position = camera_pos;

				if (KeyboardEvents::GetKeyboardState(EmberKeyCode::D))
				camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * speed;
				if (KeyboardEvents::GetKeyboardState(EmberKeyCode::A))
					camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * speed;
				if (KeyboardEvents::GetKeyboardState(EmberKeyCode::W))
					camera_pos += speed * camera_front;
				if (KeyboardEvents::GetKeyboardState(EmberKeyCode::S))
					camera_pos -= speed * camera_front;

			float xoffset = Events::MousePosition().x - last_mouse_position.x;
			float yoffset = last_mouse_position.y - Events::MousePosition().y;
			last_mouse_position = { Events::MousePosition().x, Events::MousePosition().y };

			const float sensitivity = 0.1f;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw += xoffset;
			pitch += yoffset;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			glm::vec3 direction;
			direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			direction.y = sin(glm::radians(pitch));
			direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			camera_front = glm::normalize(direction);
		}

		cursor_handler.Update();
	}

	void PerspectiveCameraController::OnEvent(Event& event) {
		EventDispatcher dispatcher(&event);
		if (!freeze) {
			dispatcher.Dispatch<KeyboardEvents>(EMBER_BIND_FUNC(KeyboardHandler));
			dispatcher.Dispatch<MouseWheelEvents>(EMBER_BIND_FUNC(MouseWheelHandler));
		}
		dispatcher.Dispatch<ResizeEvent>(EMBER_BIND_FUNC(WindowResizeHandler));
	}

	void PerspectiveCameraController::MouseWheelHandler(MouseWheelEvents& mousewheel) {
		fov -= mousewheel.direction;
		if (fov < SMALLEST_ZOOM)
			fov = SMALLEST_ZOOM;
		if (fov > HIGHEST_ZOOM)
			fov = HIGHEST_ZOOM;
		camera.SetProjection(glm::radians(fov), aspect_ratio);
	}

	void PerspectiveCameraController::WindowResizeHandler(ResizeEvent& resize) {
		aspect_ratio = (float)resize.w / (float)resize.h;
		camera.SetProjection(glm::radians(fov), aspect_ratio);
	}

	void PerspectiveCameraController::SetFreeze(bool freeze) {
		this->freeze = freeze;

		if (this->freeze)
			cursor_handler.Display();
	}

	void PerspectiveCameraController::KeyboardHandler(KeyboardEvents& keyboard) {
		if (keyboard.scancode == EmberKeyCode::M && keyboard.pressed) {
			in_camera_mode = !in_camera_mode;
			last_mouse_position = { Events::MousePosition().x, Events::MousePosition().y };
			if (!in_camera_mode)
				cursor_handler.Display();
			else
				cursor_handler.Hide();
		}
	}
}