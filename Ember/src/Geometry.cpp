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

	glm::mat4 Geometry::GetModelMatrix(const glm::vec3& position, const glm::vec3& scalar) {
		return glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { scalar.x, scalar.y, 1.0f });
	}

	glm::mat4 Geometry::GetRotatedModelMatrix(const glm::vec3& position, const glm::vec3& scalar, const glm::vec3& rotation_orientation, float degree) {
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z });
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), { scalar.x, scalar.y, scalar.z });
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(degree), rotation_orientation);

		return (trans * rotate * scale);
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
		glm::mat4 model = GetModelMatrix(position, { scalar.x, scalar.y, 1.0f });
		Mesh m = CreateGeometry(model, color, -1.0f, TEX_COORDS, QUAD_VERTEX_COUNT, QUAD_POSITIONS);

		AddIndices(m);
		gd->Submit(m);
	}

	void Quad::DrawQuad(const glm::vec3& position, const glm::vec2& scalar, uint32_t texture, const glm::vec4& color) {
		glm::mat4 model = GetModelMatrix(position, { scalar.x, scalar.y, 1.0f });
		Mesh m = CreateGeometry(model, color, (float) texture, TEX_COORDS, QUAD_VERTEX_COUNT, QUAD_POSITIONS);

		AddIndices(m);
		gd->Submit(m);
	}

	void Quad::DrawQuad(const glm::vec3& position, float degree, const glm::vec3& orientation, const glm::vec2& scalar, const glm::vec4& color) {
		glm::mat4 mat = GetRotatedModelMatrix(position, { scalar.x, scalar.y, 1.0f }, orientation, degree);
		Mesh m = CreateGeometry(mat, color, -1.0f, TEX_COORDS, QUAD_VERTEX_COUNT, QUAD_POSITIONS);

		AddIndices(m);
		gd->Submit(m);
	}

	void Quad::DrawQuad(const QuadModel& model) {
		glm::mat4 mat = GetRotatedModelMatrix(model.position, model.scalar, model.orientation, model.degree);
		Mesh m = CreateGeometry(mat, model.color, model.texture_id, model.tex_coords, QUAD_VERTEX_COUNT, QUAD_POSITIONS);

		AddIndices(m);
		gd->Submit(m);
	}

	void Quad::AddIndices(Mesh& mesh) {
		mesh.indices.push_back(AddIndice(0));
		mesh.indices.push_back(AddIndice(1));
		mesh.indices.push_back(AddIndice(2));
		mesh.indices.push_back(AddIndice(2));
		mesh.indices.push_back(AddIndice(3));
		mesh.indices.push_back(AddIndice(0));

		UpdateIndexOffset(4);
	}
}