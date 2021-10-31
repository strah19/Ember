#include "Geometry.h"

namespace Ember {
	Mesh Geometry::CreateGeometry(const glm::mat4& matrix, const glm::vec4& color, float texture_id, const glm::vec2 tex_coords[], uint32_t vertex_count, const glm::vec4 positions[]) {
		Mesh mesh;
		for (size_t i = 0; i < vertex_count; i++) {
			Vertex vertex;
			vertex.position = matrix * positions[i];
			vertex.color = color;
			vertex.texture_coordinates = tex_coords[i];
			vertex.texture_id = texture_id;
			vertex.material_id = (float)0;

			mesh.vertices.push_back(vertex);
		}

		return mesh;
	}

	glm::mat4 Geometry::GetModelMatrix(const glm::vec3& position, const glm::vec2& scalar) {
		return glm::translate(glm::mat4(1.0f), { position.x + (scalar.x / 2), position.y + (scalar.y / 2), position.z }) * glm::scale(glm::mat4(1.0f), { scalar.x, scalar.y, 1.0f });
	}

	uint32_t Geometry::AddIndice(uint32_t offset) {
		return offset + index_offset;
	}

	void Geometry::UpdateIndexOffset(uint32_t count) {
		index_offset += count;
	}

	Quad* Quad::GetQuad() {
		if (!quad) 
			quad = new Quad;	
		return quad;
	}

	void Quad::Update(EmberVertexGraphicsDevice* gd) {
		if (gd)
			this->gd = gd;

		index_offset = 0;
	}

	Quad::Quad() {
		quad = nullptr;
	}

	Quad::~Quad() {
		delete quad;
	}

	void Quad::DrawQuad(const glm::vec3& position, const glm::vec2& scalar, const glm::vec4& color) {
		glm::mat4 model = GetModelMatrix(position, scalar);
		Mesh m = CreateGeometry(model, color, -1.0f, TEX_COORDS, QUAD_VERTEX_COUNT, QUAD_POSITIONS);

		m.indices.push_back(AddIndice(0));
		m.indices.push_back(AddIndice(1));
		m.indices.push_back(AddIndice(2));
		m.indices.push_back(AddIndice(2));
		m.indices.push_back(AddIndice(3));
		m.indices.push_back(AddIndice(0));

		UpdateIndexOffset(4);
		gd->Submit(m);
	}
}