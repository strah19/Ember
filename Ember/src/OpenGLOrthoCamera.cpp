#include "OpenGLOrthoCamera.h"

namespace EmberGL {
	OrthoCamera::OrthoCamera(float left, float right, float bottom, float top) {
		SetProjection(left, right, bottom, top);
	}

	void OrthoCamera::SetProjection(float left, float right, float bottom, float top) {
		projection_matrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		CalculateView();
	}
}