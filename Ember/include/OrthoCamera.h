#ifndef ORTHO_CAMERA_H
#define ORTHO_CAMERA_H

#include "Camera.h"

namespace Ember {
	class OrthoCamera : public Camera {
	public:
		OrthoCamera(float left, float right, float bottom, float top);
		OrthoCamera() = default;

		void SetProjection(float left, float right, float bottom, float top);

		inline float Left() const { return left; }
		inline float Right() const { return right; }
		inline float Bottom() const { return bottom; }
		inline float Top() const { return top; }
	private:
		float left = 0, right = 0, bottom = 0, top = 0;
	};
}

#endif // !ORTHO_CAMERA_H
