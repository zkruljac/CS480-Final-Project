#ifndef LIGHT_H
#define LIGHT_H

#include <vector>
#include "graphics_headers.h"


class Light
{
public:
	Light(glm::mat4 viewMatrix);
	Light(glm::mat4 viewMatrix, glm::vec4 lightAmb, glm::vec4 lightDiff, glm::vec4 lightSpec, glm::vec4 gAmb);

	void SetViewSpacePosition(glm::mat4 viewMatrix);

	glm::vec4 m_globalAmbient = {0.0, 0.0, 0.0, 1.0};

	glm::vec4 m_lightAmbient = { 0.0, 0.0, 0.0, 1.0 };
	glm::vec4 m_lightDiffuse = { 1.0, 1.0, 1.0, 1.0 };
	glm::vec4 m_lightSpecular = { 1.0, 1.0, 1.0, 1.0 };

	glm::vec4 m_lightAmbient1 = { 0.0, 0.0, 0.0, 1.0 };
	glm::vec4 m_lightDiffuse1 = { 1.0, 1.0, 1.0, 1.0 };
	glm::vec4 m_lightSpecular1 = { 1.0, 1.0, 1.0, 1.0 };

	glm::vec3 m_lightPositionViewSpace;

	glm::vec3 m_lightPosition = glm::uvec3(0.0f, 0.0f, 0.0f);

};



#endif
