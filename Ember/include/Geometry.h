#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Renderer.h"

namespace Ember {
	class Geometry {
	public:
		Mesh CreateGeometry(const glm::mat4& matrix, const glm::vec4& color, float texture_id, const glm::vec2 tex_coords[], uint32_t vertex_count, const glm::vec4 positions[]);
		glm::mat4 GetModelMatrix(const glm::vec3& position, const glm::vec2& scalar);
		void UpdateIndexOffset(uint32_t count);
		uint32_t AddIndice(uint32_t offset);
	protected:
		uint32_t index_offset = 0;
	};

	class Quad : Geometry {
	public:
		 static Quad* GetQuad();
		void Update(EmberVertexGraphicsDevice* gd);
		void DrawQuad(const glm::vec3& position, const glm::vec2& scalar, const glm::vec4& color);

	private:
		inline static Quad* quad;
		EmberVertexGraphicsDevice* gd = nullptr;

		Quad();
		~Quad();
	};
}

#endif // !GEOMETRY_H