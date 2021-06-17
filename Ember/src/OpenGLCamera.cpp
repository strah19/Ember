#include "OpenGLCamera.h"

namespace EmberGL {
	void Camera::CalculateView() {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));

		view_matrix = glm::inverse(transform);
	}
}