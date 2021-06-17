#ifndef CAMERA_H
#define CAMERA_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

namespace EmberGL {
	class Camera {
	public:
		inline glm::mat4 GetProjection() const { return projection_matrix; }
		inline glm::mat4 GetView() const { return view_matrix; }
		inline float GetRotation() const { return rotation; }
		inline glm::vec3 GetPosition() const { return position; }

		inline void SetMatrixProjection(const glm::mat4& projection) { projection_matrix = projection; }
		inline void SetMatrixView(const glm::mat4& view) { view_matrix = view; }
		inline void SetRotation(float r) { rotation = r; CalculateView(); }
		inline void SetPosition(const glm::vec3& pos) { position = pos; CalculateView(); }
	protected:
		void CalculateView();
	protected:
		glm::vec3 position = { 0, 0, 3 };

		glm::mat4 projection_matrix = glm::mat4(1.0f);
		glm::mat4 view_matrix = glm::mat4(1.0f);

		float rotation = 0.0f;
	};
}

#endif // !CAMERA_H
