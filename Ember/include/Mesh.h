#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm.hpp>
#include <Texture.h>
#include <string>

namespace Ember {
	struct Vertex {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texture_coordinates;
		float texture_id;
		float material_id;
	};

	struct Mesh {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		Mesh() = default;
		Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	};

	struct ModelVertex : public Vertex {
		glm::vec3 normals = glm::vec3(0, 0, 0);
	};
}

#endif // !MESH_H
