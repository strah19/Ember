#include "OpenGLPerspectiveCamera.h"

namespace EmberGL {
	PerspectiveCamera::PerspectiveCamera(float fov, float aspect_ratio) {
		SetProjection(fov, aspect_ratio);
	}

	void PerspectiveCamera::SetProjection(float fov, float aspect_ratio) {
		CalculateView();
		projection_matrix = glm::perspective(fov, aspect_ratio, 0.1f, 100.0f);
	}
}