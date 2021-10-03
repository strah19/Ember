#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm.hpp>
#include <Texture.h>
#include <string>

namespace Ember {
	struct MeshVertex {
		glm::vec3 position = glm::vec3(0, 0, 0);
		glm::vec2 texture_coordinates = glm::vec2(0, 0);
		glm::vec3 normals = glm::vec3(0, 0, 0);
	};

	struct MeshTexture {
		Texture* texture;
		std::string path;
	};

	struct Mesh {
		std::vector<MeshVertex> vertices;
		std::vector<uint32_t> indices;

		Mesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices);
	};
}

#endif // !MESH_H
