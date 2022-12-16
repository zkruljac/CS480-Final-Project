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

void Camera::Update(glm::mat4 model, glm::vec3 translation, float rotation)
{
	angle += rotation;
	//view = glm::lookAt(glm::vec3(0.0, 0.2, -1.0) + translation, glm::vec3(0.0, 0.2, 0.0) + translation, glm::vec3(0.0, 1.0, 0.0));
	//view *= glm::rotate(glm::mat4(1.f), rotation, glm::vec3(0.0, 1.0, 0.0));
	//view = glm::translate(view, glm::vec3(cos(rotation),0, -cos(rotation)));
	//view = glm::translate(view, glm::vec3(cos(speed[0] * rotation) * dist[0], sin(speed[1] * rotation) * dist[1], sin(speed[2] * rotation) * dist[2]));
	view = glm::lookAt(glm::vec3(cos(-speed[0] * angle) * dist[0] + translation[0], 0.2 + translation[1], sin(-speed[2] * angle) * dist[2] + translation[2]), glm::vec3(0.0, 0.2, 0.0) + translation, glm::vec3(0.0, 1.0, 0.0));
}

glm::mat4 Camera::GetProjection()
{
	return projection;
}

glm::mat4 Camera::GetView()
{
	return view;
}


