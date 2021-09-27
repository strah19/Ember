#ifndef OPEN_RENDERER_H
#define OPEN_RENDERER_H

#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Camera.h"
#include "Material.h"
#include "Font.h"

namespace Ember {
	struct Vertex {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texture_coordinates;
		float texture_id;
		glm::vec3 normals = glm::vec3(0, 0, 0);
		float material_id;
	};

	constexpr size_t MAX_QUAD_COUNT = 100000;
	constexpr size_t QUAD_VERTEX_COUNT = 4;
	constexpr size_t MAX_VERTEX_COUNT = MAX_QUAD_COUNT * QUAD_VERTEX_COUNT;
	constexpr size_t MAX_INDEX_COUNT = MAX_QUAD_COUNT * 6;
	constexpr size_t CUBE_FACES = 6;
	constexpr size_t MAX_TEXTURE_SLOTS = 32;
	constexpr size_t MAX_DRAW_COMMANDS = 1000;
	constexpr size_t MAX_INSTANCE_COUNT = 10000;
	constexpr size_t MAX_MATERIAL_COUNT = 64;
	constexpr size_t MAX_LIGHT_COUNT = 64;
	constexpr glm::vec2 TEX_COORDS[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	constexpr glm::vec4 QUAD_POSITIONS[QUAD_VERTEX_COUNT] = {
		{ -0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f,  0.5f, 0.0f, 1.0f },
		{ -0.5f,  0.5f, 0.0f, 1.0f }
	};

	constexpr size_t TRIANGLE_VERTEX_COUNT = 3;
	constexpr glm::vec4 TRIANGLE_POSITIONS[TRIANGLE_VERTEX_COUNT] = {
		{ -0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.0f,  0.5f, 0.0f, 1.0f }
	};

	constexpr size_t CUBE_VERTEX_COUNT = 24;
	constexpr glm::vec3 CUBE_POSITIONS[CUBE_VERTEX_COUNT] = {
		{ -0.5f, -0.5f, 0.0f }, { 0.5f, -0.5f, 0.0f },{ 0.5f,  0.5f, 0.0f }, { -0.5f,  0.5f, 0.0f },
		{ -0.5f, -0.5f, -1.0f }, { 0.5f, -0.5f, -1.0f }, { 0.5f,  0.5f, -1.0f }, { -0.5f,  0.5f, -1.0f },
		{ -0.5f, -0.5f, -1.0f }, { -0.5f, -0.5f, 0.0f }, { -0.5f,  0.5f, 0.0f }, { -0.5f,  0.5f, -1.0f },
		{ 0.5f, -0.5f, -1.0f }, { 0.5f, -0.5f, 0.0f }, { 0.5f,  0.5f, 0.0f }, { 0.5f,  0.5f, -1.0f },
		{ -0.5f, 0.5f, -1.0f }, { 0.5f, 0.5f, -1.0f }, { 0.5f,  0.5f, 0.0f }, { -0.5f,  0.5f, 0.0f },
		{ -0.5f, -0.5f, -1.0f }, { 0.5f, -0.5f, -1.0f }, { 0.5f,  -0.5f, 0.0f }, { -0.5f,  -0.5f, 0.0f }
	};

	enum RenderFlags {
		None = 0x01, TopLeftCornerPos = 0x02, PolygonMode = 0x04
	};

	class Renderer {
	public:
		static void Init();
		static void Destroy();

		static void SetShaderToDefualt();
		static void InitRendererShader(Shader* shader);
		static void SetShader(Shader* shader);
		static void SetMaterialId(uint32_t material_id);
		static void SetLineThickness(uint32_t thickness);

		static uint32_t GetShaderId();

		static void BeginScene(Camera& camera, int flags = RenderFlags::None);
		static void EndScene();
		static void NewBatch();

		static void Submit(VertexArray* vertex_array, IndexBuffer* index_buffer, Shader* shader);

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Texture* texture, const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, uint32_t texture, const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Texture* texture, const glm::vec2 tex_coords[], const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec2 tex_coords[], const glm::vec4& color = { -1, -1, -1, -1 });

		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, Texture* texture, const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, const glm::vec2 tex_coords[], Texture* texture, const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, const glm::vec2 tex_coords[], const glm::vec4& color = { -1, -1, -1, -1 });

		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, Texture* texture, const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawCube(const glm::mat4& translation, const glm::vec4& color, float texture_id, const glm::vec2 tex_coords[]);

		static void DrawQuad(const glm::mat4& translation, const glm::vec4& color, float texture_id, const glm::vec2 tex_coords[]);
		static void DrawTriangle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawTriangle(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, const glm::vec4& color);
	
		static void DrawLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color, float width = 1.0f);

		static void RenderText(Font* font, const std::string& text, const glm::vec2& pos, const glm::vec2& scale, const glm::vec4& color);

		static void GoToNextDrawCommand();
		static void MakeCommand();
	private:
		static void StartBatch();
		static void Render();

		static float CalculateTextureIndex(Texture* texture);
		static float CalculateTextureIndex(uint32_t id);
		static void CalculateSquareIndices();
		static void CalculateTriangleIndices();
	};

	glm::vec3 CalculateVertexNormals(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
}

#endif