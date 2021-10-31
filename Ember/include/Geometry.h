#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Renderer.h"

namespace Ember {
	class Geometry {
	public:
		Mesh CreateGeometry(const glm::mat4& matrix, const glm::vec4& color, float texture_id, const glm::vec2 tex_coords[], uint32_t vertex_count, const glm::vec4 positions[]);
		glm::mat4 GetModelMatrix(const glm::vec3& position, const glm::vec3& scalar);
		glm::mat4 GetRotatedModelMatrix(const glm::vec3& position, const glm::vec3& scalar, const glm::vec3& rotation_orientation, float degree);
		void UpdateIndexOffset(uint32_t count);
		uint32_t AddIndice(uint32_t offset);
	protected:
		uint32_t index_offset = 0;
	};

	struct QuadModel {
		glm::vec3 position = { 0, 0, 0 };
		glm::vec3 scalar = { 0, 0, 0 };
		float texture_id = -1;
		glm::vec2* tex_coords = nullptr;
		glm::vec4 color = { 0, 0, 0, 0 };
		glm::vec3 orientation = { 0, 0, 0 };
		float degree = 0;
	};

	class Quad : Geometry {
	public:
		static Quad* GetQuad();
		void Update(EmberVertexGraphicsDevice* gd);

		void DrawQuad(const glm::vec3& position, const glm::vec2& scalar, const glm::vec4& color);
		void DrawQuad(const glm::vec3& position, const glm::vec2& scalar, uint32_t texture, const glm::vec4& color = { -1, -1, -1, -1 });
		void DrawQuad(const glm::vec3& position, float degree, const glm::vec3& orientation, const glm::vec2& scalar, const glm::vec4& color);
		void DrawQuad(const QuadModel& model);
	private:
		inline static Quad* quad;
		EmberVertexGraphicsDevice* gd = nullptr;

		void AddIndices(Mesh& mesh);
		Quad();
		~Quad();
	};
}

#endif // !GEOMETRY_H