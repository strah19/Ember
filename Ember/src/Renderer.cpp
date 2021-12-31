#include "Renderer.h"
#include "Logger.h"
#include "RendererCommands.h"
#include "TextureAtlas.h"
#include <gtc/matrix_transform.hpp>
#include <glad/glad.h>

namespace Ember {
	void DeviceStatistics::Reset() {
		num_of_indices = 0;
		num_of_vertices = 0;
		draw_count = 0;
	}

	template <typename V>
	GraphicsDevice<V>::GraphicsDevice(uint32_t max_vertex_count, uint32_t max_index_count) {
		vbo = new VertexBuffer(sizeof(V) * max_vertex_count);
		vao = new VertexArray();
		ds.max_vertex_count = max_vertex_count;

		vert_base = new V[max_vertex_count];
		indx_base = new uint32_t[max_vertex_count];

		ibo = new IndexBuffer(sizeof(uint32_t) * max_index_count);
		ds.max_index_count = max_index_count;

		vao->SetIndexBufferSize(ibo->GetCount());
	}

	template <typename V>
	GraphicsDevice<V>::~GraphicsDevice() {
		delete vao;
		delete vbo;
		delete ibo;

		delete[] vert_base;
		delete[] indx_base;

		shader = nullptr;
	}

	EmberVertexGraphicsDevice::EmberVertexGraphicsDevice(uint32_t max_vertex_count, uint32_t max_index_count) : GraphicsDevice(max_vertex_count, max_index_count) { }

	BatchGraphicsDevice::BatchGraphicsDevice(uint32_t max_vertex_count, uint32_t max_index_count) : EmberVertexGraphicsDevice(max_vertex_count, max_index_count) {
		VertexBufferLayout layout;
		layout.AddToBuffer(VertexBufferElement(3, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(4, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(2, false, VertexShaderType::Float));
		layout.AddToBuffer(VertexBufferElement(2, false, VertexShaderType::Float));

		vbo->SetLayout(layout);
		vao->AddVertexBuffer(vbo, VertexBufferFormat::VNCVNCVNC);
	}

	void BatchGraphicsDevice::Init() {
		idb = new IndirectDrawBuffer(sizeof(commands));
	}

	BatchGraphicsDevice::~BatchGraphicsDevice() {
		delete idb;
	}

	void BatchGraphicsDevice::Setup() {
		memset(textures, NULL, sizeof(uint32_t) * MAX_TEXTURE_SLOTS);
		texture_slot_index = 0;
		ds.Reset();

		index_offset = 0;
		cmd_vertex_base = 0;
		current_draw_command_vertex_size = 0;

		vert_ptr = vert_base;
		indx_ptr = indx_base;
	}

	void BatchGraphicsDevice::AddVertex(Vertex* v) {
		*vert_ptr = *v;
		vert_ptr++;
		ds.num_of_vertices++;
	}

	void BatchGraphicsDevice::AddIndex(uint32_t index) {
		*indx_ptr = index;
		indx_ptr++;
		ds.num_of_indices++;
	}

	bool BatchGraphicsDevice::Submit(Mesh& mesh) {
		if (ds.num_of_vertices + mesh.vertices.size() > ds.max_vertex_count || ds.num_of_indices + mesh.indices.size() > ds.max_index_count)
			return false;

		for (auto& vertex : mesh.vertices) {
			if (ds.num_of_vertices >= ds.max_vertex_count)
				break;
			AddVertex(&vertex);
			index_offset++;
		}

		for (auto& index : mesh.indices) {
			if (ds.num_of_indices >= ds.max_index_count)
				break;
			AddIndex(index);
			current_draw_command_vertex_size++;
		}

		return true;
	}

	void BatchGraphicsDevice::Render() {
		vao->Bind();
		ibo->Bind();
		vbo->Bind();
		(*shader)->Bind();

		idb->Bind();
		idb->SetData(commands, sizeof(commands), 0);

		for (uint32_t i = 0; i < texture_slot_index; i++)
			if (textures[i])
				glBindTextureUnit(i, textures[i]);
		uint32_t vertex_buf_size = (uint32_t)((uint8_t*)vert_ptr - (uint8_t*)vert_base);
		uint32_t index_buf_size = (uint32_t)((uint8_t*)indx_ptr - (uint8_t*)indx_base);

		vbo->SetData(vert_base, vertex_buf_size);
		ibo->SetData(indx_base, index_buf_size);

		vao->SetIndexBufferSize(ibo->GetCount());

		RendererCommand::DrawMultiIndirect(nullptr, ds.draw_count + 1, 0);
	}

	void BatchGraphicsDevice::NextCommand() {
		ds.draw_count++;
		cmd_vertex_base += ds.num_of_vertices;
		current_draw_command_vertex_size = 0;
	}

	void BatchGraphicsDevice::MakeCommand() {
		commands[ds.draw_count].vertex_count = current_draw_command_vertex_size;
		commands[ds.draw_count].instance_count = 1;
		commands[ds.draw_count].first_index = 0;
		commands[ds.draw_count].base_vertex = cmd_vertex_base;
		commands[ds.draw_count].base_instance = ds.draw_count;
	}

	float BatchGraphicsDevice::CalculateTextureIndex(uint32_t id) {
		float texture_id = -1.0f;

		for (uint32_t i = 0; i < texture_slot_index; i++)
			if (textures[i] == id)
				texture_id = (float)i;

		if (texture_id == -1.0f) {
			textures[texture_slot_index] = id;
			texture_id = (float)texture_slot_index;
			texture_slot_index++;

			if (texture_slot_index == MAX_TEXTURE_SLOTS) {
				EMBER_LOG_ERROR("Too many textures in one batch. Create a new draw call!");
				return -1.0f;
			}
		}

		return texture_id;
	}

	RendererFrame::~RendererFrame() {
		camera = nullptr;
	}

	Renderer::Renderer() {
		default_shader.Init("shaders/default_shader.glsl");
		InitRendererShader(&default_shader);

		gd = new BatchGraphicsDevice(MAX_VERTEX_COUNT, MAX_INDEX_COUNT);
		gd->Init();
		ssbo = new ShaderStorageBuffer(sizeof(glm::mat4), 0);
	}

	void Renderer::InitRendererShader(Shader* shader) {
		shader->Bind();
		int sampler[MAX_TEXTURE_SLOTS];
		for (int i = 0; i < MAX_TEXTURE_SLOTS; i++)
			sampler[i] = i;
		shader->SetIntArray("textures", sampler, MAX_TEXTURE_SLOTS);
	}

	void Renderer::BeginScene(Camera* camera) {
		this->camera = camera;
		proj_view = camera->GetProjection() * camera->GetView();
		current_shader = &default_shader;
		gd->SetShader(current_shader);
		gd->Setup();
	}

	void Renderer::EndScene() {
		gd->SetShader(current_shader);
		gd->MakeCommand();
		gd->NextCommand();
		ssbo->Bind();
		ssbo->SetData((void*)&proj_view, sizeof(glm::mat4), 0);
		ssbo->BindToBindPoint();
		gd->Render();
	}

	void Renderer::Submit(Mesh& mesh) {
		if (!gd->Submit(mesh)) {
			EndScene();
			gd->Setup();
			if (!gd->Submit(mesh))
				EMBER_LOG_ERROR("Singular mesh is too big. Split it up!");
		}
	}
}