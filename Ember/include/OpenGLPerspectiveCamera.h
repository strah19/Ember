#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H

#include "OpenGLCamera.h"

namespace EmberGL {
	class PerspectiveCamera : public Camera {
	public:
		PerspectiveCamera(float fov, float aspect_ratio);
		PerspectiveCamera() = default;

		void SetProjection(float fov, float aspect_ratio);
	};
}

#endif // !PERSPECTIVE_CAMERA_H
