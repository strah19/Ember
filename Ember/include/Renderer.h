#ifndef OPEN_RENDERER_H
#define OPEN_RENDERER_H

#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Camera.h"
#include "Material.h"
#include "Font.h"

namespace Ember {
	glm::mat4 GetTranslationMatrix(const glm::vec3& position, const glm::vec2& size);
	glm::mat4 GetModelMatrix(const glm::vec3& position, const glm::vec2& size);
	glm::mat4 GetRotatedModelMatrix(const glm::vec3& position, const glm::vec2& size, const glm::vec3& rotation_orientation, float degree);

	struct Vertex {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texture_coordinates;
		float texture_id;
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

	constexpr size_t CUBE_VERTEX_COUNT = 24;
	constexpr glm::vec4 CUBE_POSITIONS[CUBE_VERTEX_COUNT] = {
		{ -0.5f, -0.5f, 0.0f, 1.0f }, { 0.5f, -0.5f, 0.0f, 1.0f },{ 0.5f,  0.5f, 0.0f, 1.0f }, { -0.5f,  0.5f, 0.0f, 1.0f },
		{ -0.5f, -0.5f, -1.0f, 1.0f }, { 0.5f, -0.5f, -1.0f, 1.0f }, { 0.5f,  0.5f, -1.0f, 1.0f }, { -0.5f,  0.5f, -1.0f, 1.0f },
		{ -0.5f, -0.5f, -1.0f, 1.0f }, { -0.5f, -0.5f, 0.0f, 1.0f }, { -0.5f,  0.5f, 0.0f, 1.0f }, { -0.5f,  0.5f, -1.0f, 1.0f },
		{ 0.5f, -0.5f, -1.0f, 1.0f }, { 0.5f, -0.5f, 0.0f, 1.0f }, { 0.5f,  0.5f, 0.0f, 1.0f }, { 0.5f,  0.5f, -1.0f, 1.0f },
		{ -0.5f, 0.5f, -1.0f, 1.0f }, { 0.5f, 0.5f, -1.0f, 1.0f }, { 0.5f,  0.5f, 0.0f, 1.0f }, { -0.5f,  0.5f, 0.0f, 1.0f },
		{ -0.5f, -0.5f, -1.0f, 1.0f }, { 0.5f, -0.5f, -1.0f, 1.0f }, { 0.5f,  -0.5f, 0.0f, 1.0f }, { -0.5f,  -0.5f, 0.0f, 1.0f }
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
		static void SetPolygonLineThickness(float thickness);

		static uint32_t GetShaderId();

		static void BeginScene(Camera& camera, int flags = RenderFlags::None);
		static void EndScene();
		static void NewBatch();
		 
		static void Submit(VertexArray* vertex_array, IndexBuffer* index_buffer, Shader* shader);

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, uint32_t texture, const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, uint32_t texture, const glm::vec2 tex_coords[], const glm::vec4& color = { -1, -1, -1, -1 });

		static void DrawRotatedQuad(const glm::vec3& position, float degree, const glm::vec3& rotation_orientation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, float degree, const glm::vec3& rotation_orientation, const glm::vec2& size, uint32_t texture, const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawRotatedQuad(const glm::vec3& position, float degree, const glm::vec3& rotation_orientation, const glm::vec2& size, const glm::vec2 tex_coords[], uint32_t texture, const glm::vec4& color = { -1, -1, -1, -1 });

		static void DrawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec4& color, float width = 1.0f);
		static void RenderText(Font* font, const std::string& text, const glm::vec2& pos, const glm::vec2& scale, const glm::vec4& color);
		static void DrawShape(const glm::mat4& matrix, const glm::vec4& color, float texture_id, const glm::vec2 tex_coords[], uint32_t vertex_count, const glm::vec4 positions[]);
		static void AddIndice(uint32_t offset);
		static void UpdateIndexOffset(uint32_t count);

		static void GoToNextDrawCommand();
		static void MakeCommand();

		static void StartBatch();
		static void Render();

		static float CalculateTextureIndex(uint32_t id);
		static void CalculateSquareIndices();
	};

	class Triangle {
	public:
		static void DrawTriangle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawTriangle(const glm::vec3& position, float rotation, const glm::vec3& rotation_orientation, const glm::vec2& size, const glm::vec4& color);
		static const uint32_t VERTEX_COUNT = 3;

	private:	
		static void CalculateTriangleIndices();
	};
	constexpr glm::vec4 TRIANGLE_POSITIONS[Triangle::VERTEX_COUNT] = {
		{ -0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.0f,  0.5f, 0.0f, 1.0f }
	};

	class Cube {
	public:
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, uint32_t texture, const glm::vec4& color = { -1, -1, -1, -1 });
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, float texture_id, const glm::vec4& color = { -1, -1, -1, -1 });
	};
}

#endif