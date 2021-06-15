#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Buffer.h"
#include "Shader.h"
#include "OpenGLTexture.h"

#include <unordered_map>

namespace Ember {
	template<class T> class ResourceManager {
	public:
		void AddToManager(const std::string& name, const T& data);
		std::unordered_map<std::string, T>& GetLoadedData();
		T* SearchStorage(const std::string& name);
	protected:
		std::unordered_map<std::string, T> loaded_resources;
	};

	struct SSBOData {
		std::shared_ptr<ShaderStorageBuffer> ssbo;
		bool complete;
	};

	class SSBOManager : public ResourceManager<SSBOData> {
	public:
		void QueueForNewFrame();
		static SSBOManager* GetSSBOManager();
		virtual ~SSBOManager();
	private:
		static SSBOManager* manager;
	};

	class ShaderInfo {
	public:
		void Init(const std::string& shader);
		bool AddSSBOReference(const std::string& shader_storage_name, uint32_t size);
		std::shared_ptr<Shader>* GetShader() { return shader; }
		SSBOData* GetBufferPointer(const std::string& name);

		void UpdateSSBO(SSBOData* ssbo, uint32_t instance, void* data, uint32_t data_size);
		void UpdateSSBO(SSBOData* ssbo, void* data, uint32_t data_size, uint32_t offset);
		void SSBOUploadFinised(SSBOData* ssbo);
		void SetShader(std::shared_ptr<Shader>* shader) { this->shader = shader; }
	private:
		std::shared_ptr<Shader>* shader;
		std::vector<std::shared_ptr<ShaderStorageBuffer>*> shader_storage_refs;
	};

	class ShaderManager : public ResourceManager<std::shared_ptr<Shader>> {
	public:
		std::shared_ptr<Shader>* CreateShader(const std::string& file_path);
		static ShaderManager* GetShaderManager();
		virtual ~ShaderManager();
	private:
		static ShaderManager* manager;
	};

	class TextureManager : public ResourceManager<std::shared_ptr<Texture>> {
	public:
		std::shared_ptr<Texture>* CreateTexture(const std::string& file_path);
		static TextureManager* GetTextureManager();
		virtual ~TextureManager();
	private:
		static TextureManager* manager;
	};
}

#endif // !RESOURCE_MANAGER_H
