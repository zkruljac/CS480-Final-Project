#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
	//--Init the view and projection matrices
	//  if you will be having a moving camera the view matrix will need to more dynamic
	//  ...Like you should update it before you render more dynamic 
	//  for this project having them static will be fine

	// TO DO: Make the camera to be placed at location (10, 12, -20), point at origin, and the right-handed Y-up coordinate
	//view = glm::lookAt(glm::vec3(1.0, 20.0, -40.0), //Eye Position
		//glm::vec3(0.0, 0.0, 1.0), //Focus point
		//glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

	projection = glm::perspective(glm::radians(45.f), //the FoV typically 90 degrees is good which is what this is set to
		float(w) / float(h), //Aspect Ratio, so Circles stay Circular
		0.01f, //Distance to the near plane, normally a small value like this
		10000.0f); //Distance to the far plane, 

	//view = glm::translate(view, glm::vec3(0, 20, 40));
	// To DO:
	//Change your camera to orthographic
	//projection = glm::ortho(float(-w / 100), float(w / 100), -float(h/100), float(h/100), -20.f, 20.f);

	return true;
}

void Camera::Update(glm::mat4 model, glm::vec3 translation, float rotation, glm::mat4 Sun, glm::mat4 Mercury, glm::mat4 Venus, glm::mat4 Earth, glm::mat4 Mars, glm::mat4 Jupiter, glm::mat4 Saturn, glm::mat4 Uranus, glm::mat4 Neptune, glm::mat4 Pluto)
{

	angle += rotation;

	glm::decompose(Sun, Pscale, Protation, Stranslation, Pskew, Pperspective);
	glm::decompose(Mercury, Pscale, Protation, Metranslation, Pskew, Pperspective);
	glm::decompose(Venus, Pscale, Protation, Vtranslation, Pskew, Pperspective);
	glm::decompose(Earth, Pscale, Protation, Etranslation, Pskew, Pperspective);
	glm::decompose(Mars, Pscale, Protation, Matranslation, Pskew, Pperspective);
	glm::decompose(Jupiter, Pscale, Protation, Jtranslation, Pskew, Pperspective);
	glm::decompose(Saturn, Pscale, Protation, Satranslation, Pskew, Pperspective);
	glm::decompose(Uranus, Pscale, Protation, Utranslation, Pskew, Pperspective);
	glm::decompose(Neptune, Pscale, Protation, Ntranslation, Pskew, Pperspective);
	glm::decompose(Pluto, Pscale, Protation, Plutranslation, Pskew, Pperspective);

	if (CollisionCheck(translation, Stranslation, rotation) == 1) {}
	else if (CollisionCheck(translation, Metranslation, rotation) == 1) {}
	else if (CollisionCheck(translation, Vtranslation, rotation) == 1) {}
	else if (CollisionCheck(translation, Etranslation, rotation) == 1) {}
	else if (CollisionCheck(translation, Matranslation, rotation) == 1) {}
	else if (CollisionCheck(translation, Jtranslation, rotation) == 1){}
	else if (CollisionCheck(translation, Satranslation, rotation) == 1) {}
	else if (CollisionCheck(translation, Utranslation, rotation) == 1) {}
	else if (CollisionCheck(translation, Ntranslation, rotation) == 1) {}
	else if (CollisionCheck(translation, Plutranslation, rotation) == 1) {}

	
}

bool Camera::CollisionCheck(glm::vec3 translation, glm::vec3 Ptranslation, float rotation)
{
	if (translation.x >= Ptranslation.x - 30 && translation.x <= Ptranslation.x + 30 && translation.y >= Ptranslation.y - 30 && translation.y <= Ptranslation.y + 30 && translation.z >= Ptranslation.z - 30 && translation.z <= Ptranslation.z + 30)
	{
		colState = 1;
		if (viewState == 1)
			changeState = 1;
	}
	else
	{
		colState = 0;
		changeState = 0;
	}

	if (changeState == 1)
	{
		view = glm::lookAt(glm::vec3(cos(-speed[0] * angle) * ((translation.y*3)+10) + Ptranslation.x, translation.y+5, sin(-speed[2] * angle) * ((translation.y * 3) + 10) + Ptranslation.z), glm::vec3(0.0, 0.0, 0.0) + Ptranslation, glm::vec3(0.0, 1.0, 0.0));
		return 1;
	}
	else if (changeState == 0)
	{
		view = glm::lookAt(glm::vec3(cos(-speed[0] * angle) * dist[0] + translation.x, 0.2 + translation.y, sin(-speed[2] * angle) * dist[2] + translation.z), glm::vec3(0.0, 0.2, 0.0) + translation, glm::vec3(0.0, 1.0, 0.0));
		return 0;
	}
}

glm::mat4 Camera::GetProjection()
{
	return projection;
}

glm::mat4 Camera::GetView()
{
	return view;
}


