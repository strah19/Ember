#ifndef ORTHO_CAMERA_H
#define ORTHO_CAMERA_H

#include "Camera.h"

namespace Ember {
	class OrthoCamera : public Camera {
	public:
		OrthoCamera(float left, float right, float bottom, float top);
		OrthoCamera() = default;

		void SetProjection(float left, float right, float bottom, float top);
	};
}

#endif // !ORTHO_CAMERA_H
