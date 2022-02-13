#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Renderer.h"
#include "Font.h"

namespace Ember {
	namespace Geometry {
		Mesh CreateGeometry(const glm::mat4& matrix, const glm::vec4& color, float texture_id, const glm::vec2 tex_coords[], uint32_t vertex_count, const glm::vec4 positions[]);
		glm::mat4 GetModelMatrix(const glm::vec3& position, const glm::vec3& scalar);
		glm::mat4 GetRotatedModelMatrix(const glm::vec3& position, const glm::vec3& scalar, const glm::vec3& rotation_orientation, float degree);
		void UpdateIndexOffset(uint32_t* indices, uint32_t count);
		uint32_t AddIndice(uint32_t indices, uint32_t offset);
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

	class RendererObject {
	public:
		static void Renderer(RendererFrame* renderer);
	protected:
		static RendererFrame* renderer;
	};

	class Quad : public RendererObject {
	public:
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scalar, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scalar, uint32_t texture, const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawQuad(const glm::vec3& position, float degree, const glm::vec3& orientation, const glm::vec2& scalar, const glm::vec4& color);
		static void DrawQuad(const QuadModel& model);

		static void AddIndices(Mesh& mesh);
	private:
		static uint32_t index_offset;
	};

	class Text : public RendererObject {
	public:
		static void RenderText(Font* font, const std::string& text, const glm::vec2& position, const glm::vec2& scalar, const glm::vec4& color);
	};
}

#endif // !GEOMETRY_H