#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdint.h>
#include <glm.hpp>
#include "OpenGLTexture.h"
#include "OpenGLLight.h"

namespace EmberGL {
	struct TextureMaterial {
		std::shared_ptr<Texture>* diffuse;
		std::shared_ptr<Texture>* specular;
		float shininess;

		TextureMaterial(std::shared_ptr<Texture>& diffuse, std::shared_ptr<Texture>& specular, float shininess)
			: diffuse(&diffuse), specular(&specular), shininess(shininess) {
		}

		TextureMaterial(const TextureMaterial& material) {
			diffuse = material.diffuse;
			specular = material.specular;
			shininess = material.shininess;
		}

		TextureMaterial() = default;
	};

	struct Material { 
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;

		glm::vec4 shininess;

		Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
			: ambient(glm::vec4(ambient, 1.0)), diffuse(glm::vec4(diffuse, 1.0)), specular(glm::vec4(specular, 1.0)), shininess(glm::vec4(shininess, 1.0, 1.0, 1.0)) {
		}

		Material(const Material& material) {
			ambient = material.ambient;
			diffuse = material.diffuse;
			specular = material.specular;

			shininess = material.shininess;
		}

		Material() = default;
	};
}

#endif // !MATERIAL_H
