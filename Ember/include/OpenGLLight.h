#ifndef LIGHT_H
#define LIGHT_H

#include <glm.hpp>
#include "OpenGLShader.h"

namespace EmberGL {
	struct LightSource {
		union PosDir {
			glm::vec4 position;
			glm::vec4 direction;
		} pos_dir;

		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;

		LightSource(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& position)
			: ambient(glm::vec4(ambient, 1.0)), diffuse(glm::vec4(diffuse, 1.0)), specular(glm::vec4(specular, 1.0)), pos_dir({ glm::vec4(position, 1.0) }) { }

		LightSource(const LightSource& light_source) {
			ambient = light_source.ambient;
			diffuse = light_source.diffuse;
			specular = light_source.specular;

			pos_dir = light_source.pos_dir;
		}

		LightSource() = default;
	};

	struct LightSourcePoint {
		glm::vec4 position;
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;
		glm::vec4 attenuation;

		LightSourcePoint(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& position, float constant, float linear, float quadratic)
			: ambient(glm::vec4(ambient, 1.0)), diffuse(glm::vec4(diffuse, 1.0)), specular(glm::vec4(specular, 1.0)), position({ glm::vec4(position, 1.0) }), attenuation({ constant, linear, quadratic, 1.0 }) { }

		LightSourcePoint(const LightSourcePoint& light_source) {
			ambient = light_source.ambient;
			diffuse = light_source.diffuse;
			specular = light_source.specular;

			position = light_source.position;
			attenuation = light_source.attenuation;
		}

		LightSourcePoint() = default;
	};

	struct SpotLightSource {
		glm::vec4 position;
		glm::vec4 direction;

		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;
		glm::vec4 attenuation_cut_off;

		SpotLightSource(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& position, const glm::vec3& direction, float constant, float linear, float quadratic, float cut_off)
			: ambient(glm::vec4(ambient, 1.0)), diffuse(glm::vec4(diffuse, 1.0)), specular(glm::vec4(specular, 1.0)), position({ glm::vec4(position, 1.0) }), attenuation_cut_off({ constant, linear, quadratic, cut_off }), direction(glm::vec4(direction, 1.0)) { }

		SpotLightSource(const SpotLightSource& light_source) {
			ambient = light_source.ambient;
			diffuse = light_source.diffuse;
			specular = light_source.specular;

			position = light_source.position;
			direction = light_source.direction;
			attenuation_cut_off = light_source.attenuation_cut_off;
		}

		SpotLightSource() = default;
	};
}

#endif // !LIGHT_H
