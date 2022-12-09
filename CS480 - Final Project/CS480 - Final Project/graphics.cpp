#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height)
{
	// Used for the linux OS
#if !defined(__APPLE__) && !defined(MACOSX)
  // cout << glewGetString(GLEW_VERSION) << endl;
	glewExperimental = GL_TRUE;

	auto status = glewInit();

	// This is here to grab the error that comes from glew init.
	// This error is an GL_INVALID_ENUM that has no effects on the performance
	glGetError();

	//Check for error
	if (status != GLEW_OK)
	{
		std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
		return false;
	}
#endif



	// Init Camera
	m_camera = new Camera();
	if (!m_camera->Initialize(width, height))
	{
		printf("Camera Failed to Initialize\n");
		return false;
	}

	// Set up the shaders
	m_shader = new Shader();
	if (!m_shader->Initialize())
	{
		printf("Shader Failed to Initialize\n");
		return false;
	}

	// Add the vertex shader
	if (!m_shader->AddShader(GL_VERTEX_SHADER))
	{
		printf("Vertex Shader failed to Initialize\n");
		return false;
	}

	// Add the fragment shader
	if (!m_shader->AddShader(GL_FRAGMENT_SHADER))
	{
		printf("Fragment Shader failed to Initialize\n");
		return false;
	}

	// Connect the program
	if (!m_shader->Finalize())
	{
		printf("Program to Finalize\n");
		return false;
	}

	// Populate location bindings of the shader uniform/attribs
	if (!collectShPrLocs()) {
		printf("Some shader attribs not located!\n");
	}

	tmatM = glm::translate(glm::mat4(1.0f), glm::vec3(0, -10, 0));
	rmatM = glm::rotate(glm::mat4(1.0f), 1 * 3.14f, glm::vec3(0, 1.0f, .0f));
	smatM = glm::scale(glm::vec3(.1, .1, .1));

	// Starship
	m_mesh = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "Planetary Textures\\SpaceShip-1.obj", "Planetary Textures\\SpaceShip-1.png");

	// The Sun
	m_sun = new Sphere(64, "Planetary Textures\\2k_sun.jpg");

	// The Earth
	m_earth = new Sphere(48, "Planetary Textures\\2k_earth_daymap.jpg");
	
	// The moon
	m_luna = new Sphere(48, "Planetary Textures\\2k_moon.jpg");

	// Mercury
	m_mercury = new Sphere(48, "Planetary Textures\\Mercury.jpg");

	// Venus
	m_venus = new Sphere(48, "Planetary Textures\\VenusClouds.jpg");

	// Venus
	m_mars = new Sphere(48, "Planetary Textures\\Mars.jpg");

	// Venus
	m_jupiter = new Sphere(48, "Planetary Textures\\Jupiter.jpg");

	// Saturn
	m_saturn = new Sphere(48, "Planetary Textures\\Saturn.jpg");

	// Uranus
	m_uranus = new Sphere(48, "Planetary Textures\\Uranus.jpg");

	// Neptune
	m_neptune = new Sphere(48, "Planetary Textures\\Neptune.jpg");

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

void Graphics::HierarchicalUpdate2(double dt) {

	// Update your animation for the solar system here.

	speedM = { 0.0, 0.0, 0.0 };
	distM = { 0., 0., 0. };
	rotSpeedM = { 0.5f, 0.0f, 0.5f };
	scaleM = { 5.f, 5.f, 5.f };
	rotVectorM = glm::vec3(.0, 1.0, 0.0);


	//planet parameters
	speedS = { 0.0, 0.0, 0.0 };
	distS = { 0., 0., 0. };
	rotSpeedS = { 0.1f, 0.0f, 0.1f };
	scaleS = { 5.f, 5.f, 5.f };
	rotVectorS = glm::vec3(.0, 1.0, 0.0);

	speedP = { -0.08, 0.0, -0.08 };
	distP = { 35., 0., 35. };
	rotSpeedP = { 1.0f, 0.0f, 1.0f };
	scaleP = { 1.f, 1.f, 1.f };
	rotVectorP = glm::vec3(.3, 1.0, 0.0);

	speedP1 = { -0.4, -0.4, -0.4 };
	distP1 = { 6., 2., 6. };
	rotSpeedP1 = { .25f, 0.0f, 0.25f };
	scaleP1 = { .5f, .5f, .5f };
	rotVectorP1 = glm::vec3(.5, 1.0, 0.0);

	speedMe = { -.2, 0.0, -.2 };
	distMe = { 10.0, 0.0, 10.0 };
	rotSpeedMe = { .08f, 0.0f, .08f };
	scaleMe = { .6f, .6f, .6f };
	rotVectorMe = glm::vec3(0.0, 1.0, 0.0);

	speedV = { -.12, 0.0, -.12 };
	distV = { 20.0, 0.0, 20.0 };
	rotSpeedV = { -0.5f, -0.f, -0.5f };
	scaleV = { .9f, .9f, .9f };
	rotVectorV = glm::vec3(0.0, 1.0, 0.0);

	speedMa = { -.04, 0.0, -.04 };
	distMa = { 40.0, 0.0, 40.0 };
	rotSpeedMa = { 1.0f, 0.f, 1.0f };
	scaleMa = { .7f, .7f, .7f };
	rotVectorMa = glm::vec3(0.0, 1.0, 0.0);

	speedJ = { -.02, 0.0, -.02 };
	distJ = { 70.0, 0.0, 70.0 };
	rotSpeedJ = { .2f, .0f, .2f };
	scaleJ = { 3.f, 3.f, 3.f };
	rotVectorJ = glm::vec3(0.0, 1.0, 0.0);

	speedSa = { -.01, 0.0, -.01 };
	distSa = { 120.0, 0.0, 120.0 };
	rotSpeedSa = { .2f, .2f, .2f };
	scaleSa = { 2.5f, 2.5f, 2.5f };
	rotVectorSa = glm::vec3(0.0, 1.0, 0.1);

	speedU = { -.005, 0.0, -.005 };
	distU = { 170.0, 0.0, 170.0 };
	rotSpeedU = { .2f, .2f, .2f };
	scaleU = { 1.5f, 1.5f, 1.5f };
	rotVectorU = glm::vec3(1.0, 0.1, 0.0);

	speedN = { -.002, 0.0, -.002 };
	distN = { 200.0, 0.0, 200.0 };
	rotSpeedN = { .3f, .3f, .3f };
	scaleN = { 1.4f, 1.4f, 1.4f };
	rotVectorN = glm::vec3(.05, 1.0, 0.0);

	ComputeTransforms(dt, speedS, distS, rotSpeedS, rotVectorS, scaleS, tmatS, rmatS, smatS);
	m_sun->Update(tmatS * rmatS * smatS);

	ComputeTransforms(dt, speedP, distP, rotSpeedP, rotVectorP, scaleP, tmatP, rmatP, smatP);
	m_earth->Update(tmatP * rmatP * smatP);

	ComputeTransforms(dt, speedP1, distP1, rotSpeedP1, rotVectorP1, scaleP1, tmatP1, rmatP1, smatP1);
	m_luna->Update(tmatP * tmatP1 * rmatP1 * smatP1);

	ComputeTransforms(dt, speedMe, distMe, rotSpeedMe, rotVectorMe, scaleMe, tmatMe, rmatMe, smatMe);
	m_mercury->Update(tmatMe * rmatMe * smatMe);

	ComputeTransforms(dt, speedV, distV, rotSpeedV, rotVectorV, scaleV, tmatV, rmatV, smatV);
	m_venus->Update(tmatV * rmatV * smatV);

	ComputeTransforms(dt, speedMa, distMa, rotSpeedMa, rotVectorMa, scaleMa, tmatMa, rmatMa, smatMa);
	m_mars->Update(tmatMa * rmatMa * smatMa);

	ComputeTransforms(dt, speedJ, distJ, rotSpeedJ, rotVectorJ, scaleJ, tmatJ, rmatJ, smatJ);
	m_jupiter->Update(tmatJ * rmatJ * smatJ);

	ComputeTransforms(dt, speedSa, distSa, rotSpeedSa, rotVectorSa, scaleSa, tmatSa, rmatSa, smatSa);
	m_saturn->Update(tmatSa * rmatSa * smatSa);

	ComputeTransforms(dt, speedU, distU, rotSpeedU, rotVectorU, scaleU, tmatU, rmatU, smatU);
	m_uranus->Update(tmatU * rmatU * smatU);

	ComputeTransforms(dt, speedN, distN, rotSpeedN, rotVectorN, scaleN, tmatN, rmatN, smatN);
	m_neptune->Update(tmatN* rmatN* smatN);

	//modelStack.push();

	m_mesh->Update(dt);
	m_ship = m_mesh->GetModel();
	glm::decompose(m_ship, scale, rotation, translation, skew, perspective);
	m_camera->Update(m_ship, translation, rotation.y);
}





void Graphics::ComputeTransforms(double dt, std::vector<float> speed, std::vector<float> dist,
	std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat)
{
	tmat = glm::translate(glm::mat4(1.f), glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	rmat = glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	smat = glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
}

void Graphics::Render()
{
	//clear the screen
	glClearColor(0.0, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start the correct program
	m_shader->Enable();

	// Send in the projection and view to the shader (stay the same while camera intrinsic(perspective) and extrinsic (view) parameters are the same
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

	// Render the objects
	/*if (m_cube != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_cube->GetModel()));
		m_cube->Render(m_positionAttrib,m_colorAttrib);
	}*/

	if (m_mesh != NULL) {
		glUniform1i(m_hasTexture, false);
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mesh->GetModel()));
		if (m_mesh->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mesh->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mesh->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	/*if (m_pyramid != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_pyramid->GetModel()));
		m_pyramid->Render(m_positionAttrib, m_colorAttrib);
	}*/

	if (m_sun != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sun->GetModel()));
		if (m_sun->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sun->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_sun->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	if (m_earth != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_earth->GetModel()));
		if (m_earth->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_earth->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_earth->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}


	// Render Moon
	if (m_luna != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_luna->GetModel()));
		if (m_luna->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_luna->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_luna->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	if (m_mercury != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mercury->GetModel()));
		if (m_mercury->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mercury->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mercury->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	if (m_venus != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_venus->GetModel()));
		if (m_venus->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_venus->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_venus->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	if (m_mars != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mars->GetModel()));
		if (m_mars->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mars->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mars->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	if (m_jupiter != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_jupiter->GetModel()));
		if (m_jupiter->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_jupiter->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_jupiter->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	if (m_saturn != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_saturn->GetModel()));
		if (m_saturn->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_saturn->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_saturn->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	if (m_uranus != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_uranus->GetModel()));
		if (m_uranus->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_uranus->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_uranus->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	if (m_neptune != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_neptune->GetModel()));
		if (m_neptune->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_neptune->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_neptune->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Get any errors from OpenGL
	auto error = glGetError();
	if (error != GL_NO_ERROR)
	{
		string val = ErrorString(error);
		std::cout << "Error initializing OpenGL! " << error << ", " << val << std::endl;
	}
}


bool Graphics::collectShPrLocs() {
	bool anyProblem = true;
	// Locate the projection matrix in the shader
	m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
	if (m_projectionMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_projectionMatrix not found\n");
		anyProblem = false;
	}

	// Locate the view matrix in the shader
	m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
	if (m_viewMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_viewMatrix not found\n");
		anyProblem = false;
	}

	// Locate the model matrix in the shader
	m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
	if (m_modelMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_modelMatrix not found\n");
		anyProblem = false;
	}

	// Locate the position vertex attribute
	m_positionAttrib = m_shader->GetAttribLocation("v_position");
	if (m_positionAttrib == -1)
	{
		printf("v_position attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_colorAttrib = m_shader->GetAttribLocation("v_color");
	if (m_colorAttrib == -1)
	{
		printf("v_color attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_tcAttrib = m_shader->GetAttribLocation("v_tc");
	if (m_tcAttrib == -1)
	{
		printf("v_texcoord attribute not found\n");
		anyProblem = false;
	}

	m_hasTexture = m_shader->GetUniformLocation("hasTexture");
	if (m_hasTexture == INVALID_UNIFORM_LOCATION) {
		printf("hasTexture uniform not found\n");
		anyProblem = false;
	}

	return anyProblem;
}

std::string Graphics::ErrorString(GLenum error)
{
	if (error == GL_INVALID_ENUM)
	{
		return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
	}

	else if (error == GL_INVALID_VALUE)
	{
		return "GL_INVALID_VALUE: A numeric argument is out of range.";
	}

	else if (error == GL_INVALID_OPERATION)
	{
		return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
	}

	else if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
	{
		return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
	}

	else if (error == GL_OUT_OF_MEMORY)
	{
		return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
	}
	else
	{
		return "None";
	}
}

