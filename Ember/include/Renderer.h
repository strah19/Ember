#ifndef OPEN_RENDERER_H
#define OPEN_RENDERER_H

#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"

namespace Ember {
	constexpr uint32_t MAX_TEXTURE_SLOTS = 32;
	constexpr size_t MAX_DRAW_COMMANDS = 1000;
	constexpr size_t MAX_VERTEX_COUNT = 10000;
	constexpr size_t MAX_INDEX_COUNT = 10000;
	constexpr size_t QUAD_VERTEX_COUNT = 4;
	constexpr glm::vec2 TEX_COORDS[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	constexpr glm::vec4 QUAD_POSITIONS[QUAD_VERTEX_COUNT] = {
		{ -0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f,  0.5f, 0.0f, 1.0f },
		{ -0.5f,  0.5f, 0.0f, 1.0f }
	};

	struct DeviceStatistics {
		uint32_t num_of_vertices = 0;
		uint32_t num_of_indices = 0;
		uint32_t draw_count = 0;
		uint32_t max_vertex_count = 0;
		uint32_t max_index_count = 0;

		void Reset();
	};

	struct DrawElementsCommand {
		uint32_t vertex_count = 0;
		uint32_t instance_count = 0;
		uint32_t first_index = 0;
		uint32_t base_vertex = 0;
		uint32_t base_instance = 0;
	};

	template <typename V>
	class GraphicsDevice {
	public:
		GraphicsDevice() = default;
		GraphicsDevice(uint32_t max_vertex_count, uint32_t max_index_count);
		virtual ~GraphicsDevice();

		virtual void Init() = 0;
		virtual void Setup() = 0;
		virtual bool Submit(Mesh& mesh) = 0;
		virtual void Render() = 0;

		inline void SetShader(Shader* shader) { this->shader = &shader; }
		inline bool Empty() const { return (vert_base == vert_ptr); }

		inline uint32_t* IndexPtr() { return indx_ptr; }
	protected:
		//Pointer to another shader that is also a pointer :)
		Shader** shader = nullptr;

		VertexArray* vao = nullptr;
		VertexBuffer* vbo = nullptr;
		IndexBuffer* ibo = nullptr;

		V* vert_base = nullptr;
		V* vert_ptr = nullptr;

		uint32_t* indx_base = nullptr;
		uint32_t* indx_ptr = nullptr;

		DeviceStatistics ds;
	};

	class EmberVertexGraphicsDevice : public GraphicsDevice<Vertex> {
	public:
		EmberVertexGraphicsDevice() = default;
		EmberVertexGraphicsDevice(uint32_t max_vertex_count, uint32_t max_index_count);
	};

	class BatchGraphicsDevice : public EmberVertexGraphicsDevice {
	public:
		BatchGraphicsDevice() = default;
		BatchGraphicsDevice(uint32_t max_vertex_count, uint32_t max_index_count);
		virtual ~BatchGraphicsDevice();

		virtual void Init() override;
		virtual void Setup() override;
		virtual bool Submit(Mesh& mesh) override;
		virtual void Render() override;

		virtual void NextCommand();
		virtual void MakeCommand();

		inline uint32_t IndexOffset() const { return index_offset; }
		float CalculateTextureIndex(uint32_t id);
	private:
		IndirectDrawBuffer* idb = nullptr;

		uint32_t texture_slot_index = 0;
		uint32_t textures[MAX_TEXTURE_SLOTS] = { 0 };

		uint32_t index_offset = 0;
		uint32_t cmd_vertex_base = 0;
		uint32_t draw_command_size = 0;
		uint32_t current_draw_command_vertex_size = 0;
		DrawElementsCommand commands[MAX_DRAW_COMMANDS];

		void AddVertex(Vertex* v);
		void AddIndex(uint32_t index);
	};

	enum RenderFlags {
		None = 0x01,
		TopLeft = 0x02
	};

	class RendererFrame {
	public:
		RendererFrame() = default;
		virtual ~RendererFrame();

		virtual void BeginScene(Camera* camera) = 0;
		virtual void EndScene() = 0;
		virtual void Submit(Mesh& mesh) = 0;

		inline int GetFlags() const { return flags; }
		inline void SetFlag(int flag, bool v) { if (v) flags |= flag; else flags &= ~flag; }
		inline BatchGraphicsDevice* GetGraphicsDevice() { return gd; }
	protected:
		Camera* camera = nullptr;
		glm::mat4 proj_view = glm::mat4(1.0f);
		Shader default_shader;
		Shader* current_shader = nullptr;
		int flags = RenderFlags::None;
		BatchGraphicsDevice* gd;
	};

	class Renderer : public RendererFrame {
	public:
		Renderer();

		virtual void BeginScene(Camera* camera) override;
		virtual void EndScene() override;
		virtual void Submit(Mesh& mesh) override;

		void InitRendererShader(Shader* shader);
	private:
		ShaderStorageBuffer* ssbo;
	};
}

#endif