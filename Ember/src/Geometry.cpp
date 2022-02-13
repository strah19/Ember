#include "Geometry.h"
#include "Logger.h"
#include "TextureAtlas.h"

namespace Ember {
	using namespace Geometry;
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

	RendererFrame* Quad::renderer = nullptr;
	uint32_t Quad::index_offset = 0;

	uint32_t Geometry::AddIndice(uint32_t indices, uint32_t offset) {
		return offset + indices;
	}

	void Geometry::UpdateIndexOffset(uint32_t* indices, uint32_t count) {
		*indices += count;
	}

	void RendererObject::Renderer(RendererFrame* ren) {
		renderer = ren;
	}

	void Quad::DrawQuad(const glm::vec3& position, const glm::vec2& scalar, const glm::vec4& color) {
		glm::mat4 model = (renderer->GetFlags() & RenderFlags::TopLeft) ? GetModelMatrix({ position.x + (scalar.x / 2), position.y + (scalar.y / 2), position.z },
			glm::vec3(scalar.x, scalar.y, 1.0f)) : GetModelMatrix(position, { scalar.x, scalar.y, 1.0f });
		Mesh m = CreateGeometry(model, color, -1.0f, TEX_COORDS, QUAD_VERTEX_COUNT, QUAD_POSITIONS);

		AddIndices(m);
		renderer->Submit(m);
	}

	void Quad::DrawQuad(const glm::vec3& position, const glm::vec2& scalar, uint32_t texture, const glm::vec4& color) {
		glm::mat4 model = (renderer->GetFlags() & RenderFlags::TopLeft) ? GetModelMatrix({ position.x + (scalar.x / 2), position.y + (scalar.y / 2), position.z },
			glm::vec3(scalar.x, scalar.y, 1.0f)) : GetModelMatrix(position, { scalar.x, scalar.y, 1.0f });
		Mesh m = CreateGeometry(model, color, renderer->GetGraphicsDevice()->CalculateTextureIndex(texture), TEX_COORDS, QUAD_VERTEX_COUNT, QUAD_POSITIONS);

		AddIndices(m);
		renderer->Submit(m);
	}

	void Quad::DrawQuad(const glm::vec3& position, float degree, const glm::vec3& orientation, const glm::vec2& scalar, const glm::vec4& color) {
		glm::mat4 model = (renderer->GetFlags() & RenderFlags::TopLeft) ? GetRotatedModelMatrix({ position.x + (scalar.x / 2), position.y + (scalar.y / 2), position.z },
			glm::vec3(scalar.x, scalar.y, 1.0f), orientation, degree) : GetRotatedModelMatrix(position, { scalar.x, scalar.y, 1.0f }, orientation, degree);
		Mesh m = CreateGeometry(model, color, -1.0f, TEX_COORDS, QUAD_VERTEX_COUNT, QUAD_POSITIONS);

		AddIndices(m);
		renderer->Submit(m);
	}

	void Quad::DrawQuad(const QuadModel& model) {
		glm::mat4 mat = (renderer->GetFlags() & RenderFlags::TopLeft) ? GetRotatedModelMatrix({ model.position.x + (model.scalar.x / 2), model.position.y + (model.scalar.y / 2), model.position.z },
			model.scalar, model.orientation, model.degree) :
			GetRotatedModelMatrix(model.position, model.scalar, model.orientation, model.degree);
		Mesh m = CreateGeometry(mat, model.color, model.texture_id, model.tex_coords, QUAD_VERTEX_COUNT, QUAD_POSITIONS);

		AddIndices(m);
		renderer->Submit(m);
	}

	void Quad::AddIndices(Mesh& mesh) {
		if (renderer->GetGraphicsDevice()->Empty())
			index_offset = 0;

		mesh.indices.push_back(AddIndice(index_offset, 0));
		mesh.indices.push_back(AddIndice(index_offset, 1));
		mesh.indices.push_back(AddIndice(index_offset, 2));
		mesh.indices.push_back(AddIndice(index_offset, 2));
		mesh.indices.push_back(AddIndice(index_offset, 3));
		mesh.indices.push_back(AddIndice(index_offset, 0));

		UpdateIndexOffset(&index_offset, 4);
	}

	float norm(float v) {
		return (v / (50));
	}

	void Text::RenderText(Font* font, const std::string& text, const glm::vec2& position, const glm::vec2& scalar, const glm::vec4& color) {
		float x = position.x;
		float y = position.y;

		for (auto& c : text) {
			Glyph character = font->glyphs[c];
			float normalized_width = TextureAtlas::CalculateSpriteCoordinate({ character.size.x, 0 }, font->width, font->height).x - 0.00002f * font->size;

			float xpos = x + norm(character.bearing.x * scalar.x);
			float ypos = y - norm((character.size.y - character.bearing.y) * scalar.y);

			float w = norm(character.size.x * scalar.x);
			float h = norm(character.size.y * scalar.y);

			float clean = 0.00001f * font->size;

			glm::vec2 coords[] = {
				{ character.offset + clean, 1.0f },
				{ character.offset + normalized_width + clean, 1.0f },
				{ character.offset + normalized_width + clean, 0.0f },
				{ character.offset + clean, 0.0f }
			};

			float tex_id = renderer->GetGraphicsDevice()->CalculateTextureIndex(font->texture);

			glm::vec4 pos[] = {
				{ xpos, ypos, 0.0f, 1.0f },
				{ xpos + w,  ypos, 0.0f, 1.0f },
				{ xpos + w,  ypos + h, 0.0f, 1.0f },
				{ xpos, ypos + h, 0.0f, 1.0f },

			};

			glm::mat4 model = (renderer->GetFlags() & RenderFlags::TopLeft) ? GetModelMatrix({ position.x + (scalar.x / 2), position.y + (scalar.y / 2), 0.0f },
				glm::vec3(scalar.x, scalar.y, 1.0f)) : GetModelMatrix(glm::vec3(position.x, position.y, 0.0f), { scalar.x, scalar.y, 1.0f });

			Mesh mesh;
			for (size_t i = 0; i < QUAD_VERTEX_COUNT; i++) {
				Vertex vertex;
				vertex.position = pos[i] * model;
				vertex.color = color;
				vertex.texture_coordinates = coords[i];
				vertex.texture_id = tex_id;
				vertex.material_id = (float)0;

				mesh.vertices.push_back(vertex);
			}

			Quad::AddIndices(mesh);
			renderer->Submit(mesh);

			x += norm((character.advance.x >> 6) * scalar.x);
		}
	}
}