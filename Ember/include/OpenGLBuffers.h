#ifndef OPENGL_BUFFERS_H
#define OPENGL_BUFFERS_H

#include <memory>
#include <vector>
#include <string>

namespace EmberGL {
	enum class VertexShaderType {
		None, Float, Int
	};

	struct VertexBufferElement {
		uint32_t size;
		uint32_t index;
		VertexShaderType type;
		bool normalized;
		uint32_t offset;

		VertexBufferElement(uint32_t size, bool normalized, VertexShaderType type)
			: size(size), index(0), normalized(normalized), type(type), offset(0) { }
	};

	static uint32_t GetSizeInBytes(VertexShaderType type) {
		switch (type)
		{
		case VertexShaderType::None: return 0;
			break;
		case VertexShaderType::Float: return 4;
			break;
		case VertexShaderType::Int: return 4;
			break;
		default:
			break;
		}

		return 0;
	}

	class VertexBufferLayout {
	public:
		VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements)
			: elements(elements) {
			Calculate();
		}

		VertexBufferLayout() { }

		void AddToBuffer(const VertexBufferElement& element) {
			elements.push_back(element);
			elements.back().index = (uint32_t)(elements.size() - 1);
		}

		uint32_t Calculate() {
			stride = 0;
			for (auto& element : elements) {
				element.offset = stride;
				stride += element.size;
			}

			return stride;
		}

		std::vector<VertexBufferElement> GetLayout() const { return elements; }
		VertexBufferElement LastElement() const { return elements.back(); }

		uint32_t GetStride() const { return stride; }
	private:
		std::vector<VertexBufferElement> elements;
		uint32_t stride = 0;
	};

	class VertexBuffer  {
	public:
		VertexBuffer(uint32_t size);
		VertexBuffer(float* vertices, uint32_t size);
		virtual ~VertexBuffer();

		void Bind();
		void UnBind();
		void SetData(void* data, uint32_t size, uint32_t offset = 0);

		uint32_t GetId() const { return vertex_buffer_id; }

		void SetLayout(const VertexBufferLayout& lay) { layout = std::make_shared<VertexBufferLayout>(lay); }
		std::shared_ptr<VertexBufferLayout> GetLayout() { return layout; }
	private:
		uint32_t vertex_buffer_id;
		std::shared_ptr<VertexBufferLayout> layout;
	};

	class IndexBuffer {
	public:
		IndexBuffer(uint32_t* indices, uint32_t size);
		IndexBuffer(uint32_t size);
		virtual ~IndexBuffer();
		void SetData(uint32_t* data, uint32_t size);

		void Bind();
		void UnBind();
		uint32_t GetId() const { return index_buffer_id; }
		uint32_t GetCount() const { return count; }
	private:
		uint32_t index_buffer_id;
		uint32_t count = 0;
	};

	class UniformBuffer {
	public:
		UniformBuffer(uint32_t size);
		virtual ~UniformBuffer();

		uint32_t GetUniformBlockId(uint32_t shader_id, const std::string& block_name);
		void BindToBindPoint();
		void BindToShader(uint32_t shader_id, const std::string& block_name);
		void AllocateData(uint32_t size);

		void Bind();
		void UnBind();
		uint32_t GetId() const;
		void SetData(void* data, uint32_t size, uint32_t offset);
	private:
		uint32_t uniform_buffer_id;
		uint32_t uniform_buffer_point;
		uint32_t size_of_buffer;
	};

	class IndirectDrawBuffer {
	public:
		IndirectDrawBuffer(uint32_t size);
		virtual ~IndirectDrawBuffer();

		void Bind();
		void UnBind();
		uint32_t GetId() const;
		void SetData(void* data, uint32_t size, uint32_t offset);
		void AllocateData(uint32_t size, void* data);
	private:
		uint32_t indirect_buffer_id;
		uint32_t size_of_buffer;
	};

	class ShaderStorageBuffer {
	public:
		ShaderStorageBuffer(uint32_t size);
		virtual ~ShaderStorageBuffer();

		uint32_t GetUniformBlockId(uint32_t shader_id, const std::string& block_name);
		void BindToBindPoint();
		void BindToShader(uint32_t shader_id, const std::string& block_name);

		void Bind();
		void UnBind();
		uint32_t GetId() const;
		void SetData(void* data, uint32_t size, uint32_t offset);
		void AllocateData(uint32_t size, void* data);
	private:
		uint32_t shader_storage_id;
		uint32_t binding_point;
		uint32_t size_of_buffer;
	};
}

#endif // !OPENGL_BUFFERS_H
