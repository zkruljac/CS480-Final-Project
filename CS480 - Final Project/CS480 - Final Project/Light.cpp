#include "Light.h"

Light::Light(glm::mat4 viewMatrix)
{
	m_lightPositionViewSpace[0] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).x;
	m_lightPositionViewSpace[1] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).y;
	m_lightPositionViewSpace[2] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).z;
}

Light::Light(glm::mat4 viewMatrix, glm::vec4 lightAmb, glm::vec4 lightDiff, glm::vec4 lightSpec, glm::vec4 gAmb)
{
	//SetViewSpacePosition(viewMatrix);
	m_lightPositionViewSpace = glm::vec3(0, 0, 0);

	m_lightAmbient[0] = lightAmb[0];
	m_lightAmbient[1] = lightAmb[1];
	m_lightAmbient[2] = lightAmb[2];

	m_lightDiffuse[0] = lightDiff[0];
	m_lightDiffuse[1] = lightDiff[1];
	m_lightDiffuse[2] = lightDiff[2];

	m_lightSpecular[0] = lightSpec[0];
	m_lightSpecular[1] = lightSpec[1];
	m_lightSpecular[2] = lightSpec[2];

	m_globalAmbient[0] = gAmb[0];
	m_globalAmbient[1] = gAmb[1];
	m_globalAmbient[2] = gAmb[2];
}

void Light::SetViewSpacePosition(glm::mat4 viewMatrix)
{
	m_lightPositionViewSpace[0] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).x;
	m_lightPositionViewSpace[1] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).y;
	m_lightPositionViewSpace[2] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).z;
}