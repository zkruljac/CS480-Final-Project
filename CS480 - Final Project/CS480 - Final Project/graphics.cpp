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

	m_light = new Light(m_camera->GetView(), glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec4(0.0, 0.0, 0.0, 1.0));
	// Starship
	m_mesh = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "Planetary Textures\\SpaceShip-1.obj", "Planetary Textures\\SpaceShip-1.png");

	// Skybox
	m_skybox = new Sphere(64, "Planetary Textures\\2k_stars_milky_way.jpg");

	// The Sun
	m_sun = new Sphere(128, "Planetary Textures\\2k_sun.jpg");

	// The Earth
	m_earth = new Sphere(48, "Planetary Textures\\2k_earth_daymap.jpg", "Planetary Textures\\2k_earth_daymap-n.jpg");
	
	// The moon
	m_luna = new Sphere(48, "Planetary Textures\\2k_moon.jpg");

	// Mercury
	m_mercury = new Sphere(48, "Planetary Textures\\Mercury.jpg", "Planetary Textures\\Mercury-n.jpg");

	// Venus
	m_venus = new Sphere(48, "Planetary Textures\\VenusClouds.jpg");

	// Mars
	m_mars = new Sphere(48, "Planetary Textures\\Mars.jpg");
	// Deimos
	m_deimos = new Sphere(12, "Planetary Textures\\Ceres.jpg");
	// Phobos
	m_phobos = new Sphere(24, "Planetary Textures\\Haumea.jpg");

	// Venus
	m_jupiter = new Sphere(48, "Planetary Textures\\Jupiter.jpg");
	// Venus
	m_ganymede = new Sphere(48, "Planetary Textures\\Mercury.jpg");
	// Venus
	m_callisto = new Sphere(48, "Planetary Textures\\Haumea.jpg");
	// Venus
	m_europa = new Sphere(48, "Planetary Textures\\Eris.jpg");
	// Venus
	m_io = new Sphere(48, "Planetary Textures\\Ceres.jpg");

	// Saturn
	m_saturn = new Sphere(48, "Planetary Textures\\Saturn.jpg");
	// Saturn
	m_ring = new Sphere(48, "Planetary Textures\\Saturn_ring.png");
	// Saturn
	m_titan = new Sphere(24, "Planetary Textures\\VenusClouds.jpg");

	// Uranus
	m_uranus = new Sphere(48, "Planetary Textures\\Uranus.jpg");

	// Neptune
	m_neptune = new Sphere(48, "Planetary Textures\\Neptune.jpg");

	// Pluto
	m_pluto = new Sphere(48, "Planetary Textures\\Ceres.jpg");

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

void Graphics::HierarchicalUpdate2(double dt) {

	// Update your animation for the solar system here.

	//Ship
	speedM = { 0.0, 0.0, 0.0 };
	distM = { 0., 0., 0. };
	rotSpeedM = { 0.5f, 0.0f, 0.5f };
	scaleM = { 5.0f, 5.0f, 5.0f };
	rotVectorM = glm::vec3(.0, 1.0, 0.0);

	//Skybox
	speedSB = { 0.0, 0.0, 0.0 };
	distSB = { 0., 0., 0. };
	rotSpeedSB = { 0.001f, 0.0f, 0.001f };
	scaleSB = { 2000.f, 2000.f, 2000.f };
	rotVectorSB = glm::vec3(.0, 1.0, 0.0);


	//planet parameters
	speedS = { 0.0, 0.0, 0.0 };
	distS = { 0., 0., 0. };
	rotSpeedS = { 0.02f, 0.0f, 0.02 };
	scaleS = { 10.f, 10.f, 10.f };
	rotVectorS = glm::vec3(.0, 1.0, 0.0);

	speedMe = { -.01, 0.0, -.01 };
	distMe = { 80.0, 0.0, 80.0 };
	rotSpeedMe = { .04f, 0.0f, .04f };
	scaleMe = { 1.2f, 1.2f, 1.2f };
	rotVectorMe = glm::vec3(0.0, 1.0, 0.0);

	speedV = { -.006, 0.0, -.006 };
	distV = { -160.0, 0.0, -160.0 };
	rotSpeedV = { -0.25f, -0.f, -0.25f };
	scaleV = { 1.8f, 1.8f, 1.8f };
	rotVectorV = glm::vec3(0.0, 1.0, 0.0);

	speedE = { -0.004, 0.0, -0.004 };
	distE = { 280., 0., 280. };
	rotSpeedE = { 0.5f, 0.0f, 0.5f };
	scaleE = { 2.f, 2.f, 2.f };
	rotVectorE = glm::vec3(.3, 1.0, 0.0);

	speedL = { -0.2, -0.2, -0.2 };
	distL = { 24., 4., 24. };
	rotSpeedL = { .125f, 0.0f, 0.125f };
	scaleL = { 1.0f, 1.0f, 1.0f };
	rotVectorL = glm::vec3(.5, 1.0, 0.0);

	speedMa = { -.002, 0.0, -.002 };
	distMa = { 360.0, 0.0, 360.0 };
	rotSpeedMa = { 0.5f, 0.f, 0.5f };
	scaleMa = { 1.4f, 1.4f, 1.4f };
	rotVectorMa = glm::vec3(0.0, 1.0, 0.0);

	speedDei = { -.2, -0.1, -.2 };
	distDei = { 8.0, 1.0, 8.0 };
	rotSpeedDei = { 0.2f, 0.f, 0.2f };
	scaleDei = { 0.1f, 0.2f, 0.15f };
	rotVectorDei = glm::vec3(0.0, 1.0, 0.5);

	speedPhob = { -.5, -0.1, -.5 };
	distPhob = { 5.0, 1.0, 5.0 };
	rotSpeedPhob = { 0.5f, 0.f, 0.5f };
	scalePhob = { 0.2f, 0.15f, 0.2f };
	rotVectorPhob = glm::vec3(0.0, 1.0, 0.2);

	speedJ = { -.001, 0.0, -.001 };
	distJ = { -600.0, 0.0, -600.0 };
	rotSpeedJ = { .1f, .0f, .1f };
	scaleJ = { 6.f, 6.f, 6.f };
	rotVectorJ = glm::vec3(0.0, 1.0, 0.0);

	speedGany = { -.25, 0.0, -.25 };
	distGany = { -20.0, 0.0, -20.0 };
	rotSpeedGany = { .1f, .0f, .1f };
	scaleGany = { 1.f, 1.f, 1.f };
	rotVectorGany = glm::vec3(0.0, 1.0, 0.0);

	speedCall = { -.1, 0.0, -.1 };
	distCall = { -45.0, 0.0, -45.0 };
	rotSpeedCall = { .1f, .0f, .1f };
	scaleCall = { .7f, .7f, .7f };
	rotVectorCall = glm::vec3(0.0, 1.0, 0.0);

	speedEur = { -.15, 0.0, -.15 };
	distEur = { 30.0, 0.0, 30.0 };
	rotSpeedEur = { .1f, .0f, .1f };
	scaleEur = { .6f, .6f, .6f };
	rotVectorEur = glm::vec3(0.0, 1.0, 0.0);

	speedIo = { -.3, 0.0, -.3 };
	distIo = { -10.0, 0.0, -10.0 };
	rotSpeedIo = { .5f, .0f, .5f };
	scaleIo = { .5f, .5f, .5f };
	rotVectorIo = glm::vec3(0.0, 1.0, 0.0);

	speedSa = { -.0005, 0.0, -.0005 };
	distSa = { 800.0, 0.0, 800.0 };
	rotSpeedSa = { .02f, .02f, .02f };
	scaleSa = { 5.0f, 5.0f, 5.0f };
	rotVectorSa = glm::vec3(0.0, 1.0, 0.1);

	scaleRing = { 15.0f, .1f, 15.0f };

	speedTitan = { -.1, -.1, -.1 };
	distTitan = { 40.0, 5.0, 40.0 };
	rotSpeedTitan = { .2f, .2f, .2f };
	scaleTitan = { 1.3f, 1.3f, 1.3f };
	rotVectorTitan = glm::vec3(0.0, 1.0, 0.1);

	speedU = { -.00025, 0.0, -.00025 };
	distU = { -1100.0, 0.0, -1100.0 };
	rotSpeedU = { .1f, .1f, .1f };
	scaleU = { 3.0f, 3.0f, 3.0f };
	rotVectorU = glm::vec3(1.0, 0.1, 0.0);

	speedN = { -.0001, 0.0, -.0001 };
	distN = { 1400.0, 0.0, 1400.0 };
	rotSpeedN = { .1f, .1f, .1f };
	scaleN = { 2.8f, 2.8f, 2.8f };
	rotVectorN = glm::vec3(.05, 1.0, 0.0);

	speedPlu = { -.0001, 0.0, -.0001 };
	distPlu = { 1500.0, 0.0, 1300.0 };
	rotSpeedPlu = { .1f, .1f, .1f };
	scalePlu = { 1.1f, 1.1f, 1.1f };
	rotVectorPlu = glm::vec3(.05, 1.0, 0.0);

	ComputeTransforms(dt, speedS, distS, rotSpeedS, rotVectorS, scaleS, tmatS, rmatS, smatS);
	m_sun->Update(tmatS * rmatS * smatS);

	ComputeTransforms(dt, speedSB, distSB, rotSpeedSB, rotVectorSB, scaleSB, tmatSB, rmatSB, smatSB);
	m_skybox->Update(tmatSB* rmatSB * smatSB);

	ComputeTransforms(dt, speedE, distE, rotSpeedE, rotVectorE, scaleE, tmatE, rmatE, smatE);
	m_earth->Update(tmatE * rmatE * smatE);

	ComputeTransforms(dt, speedL, distL, rotSpeedL, rotVectorL, scaleL, tmatL, rmatL, smatL);
	m_luna->Update(tmatE * tmatL * rmatL * smatL);

	ComputeTransforms(dt, speedMe, distMe, rotSpeedMe, rotVectorMe, scaleMe, tmatMe, rmatMe, smatMe);
	m_mercury->Update(tmatMe * rmatMe * smatMe);

	ComputeTransforms(dt, speedV, distV, rotSpeedV, rotVectorV, scaleV, tmatV, rmatV, smatV);
	m_venus->Update(tmatV * rmatV * smatV);

	ComputeTransforms(dt, speedMa, distMa, rotSpeedMa, rotVectorMa, scaleMa, tmatMa, rmatMa, smatMa);
	m_mars->Update(tmatMa * rmatMa * smatMa);
	ComputeTransforms(dt, speedDei, distDei, rotSpeedDei, rotVectorDei, scaleDei, tmatDei, rmatDei, smatDei);
	m_deimos->Update(tmatMa * tmatDei* rmatDei* smatDei);
	ComputeTransforms(dt, speedPhob, distPhob, rotSpeedPhob, rotVectorPhob, scalePhob, tmatPhob, rmatPhob, smatPhob);
	m_phobos->Update(tmatMa * tmatPhob * rmatPhob* smatPhob);

	ComputeTransforms(dt, speedJ, distJ, rotSpeedJ, rotVectorJ, scaleJ, tmatJ, rmatJ, smatJ);
	m_jupiter->Update(tmatJ * rmatJ * smatJ);
	ComputeTransforms(dt, speedGany, distGany, rotSpeedGany, rotVectorGany, scaleGany, tmatGany, rmatGany, smatGany);
	m_ganymede->Update(tmatJ* tmatGany* rmatGany* smatGany);
	ComputeTransforms(dt, speedCall, distCall, rotSpeedCall, rotVectorCall, scaleCall, tmatCall, rmatCall, smatCall);
	m_callisto->Update(tmatJ* tmatCall* rmatCall* smatCall);
	ComputeTransforms(dt, speedEur, distEur, rotSpeedEur, rotVectorEur, scaleEur, tmatEur, rmatEur, smatEur);
	m_europa->Update(tmatJ* tmatEur* rmatEur* smatEur);
	ComputeTransforms(dt, speedIo, distIo, rotSpeedIo, rotVectorIo, scaleIo, tmatIo, rmatIo, smatIo);
	m_io->Update(tmatJ* tmatIo* rmatIo* smatIo);

	ComputeTransforms(dt, speedSa, distSa, rotSpeedSa, rotVectorSa, scaleSa, tmatSa, rmatSa, smatSa);
	m_saturn->Update(tmatSa * rmatSa * smatSa);
	ComputeTransforms(dt, speedSa, distSa, rotSpeedSa, rotVectorSa, scaleRing, tmatSa, rmatSa, smatRing);
	m_ring->Update(tmatSa* rmatSa* smatRing);
	ComputeTransforms(dt, speedTitan, distTitan, rotSpeedTitan, rotVectorTitan, scaleTitan, tmatTitan, rmatTitan, smatTitan);
	m_titan->Update(tmatSa* tmatTitan* rmatTitan* smatTitan);


	ComputeTransforms(dt, speedU, distU, rotSpeedU, rotVectorU, scaleU, tmatU, rmatU, smatU);
	m_uranus->Update(tmatU * rmatU * smatU);

	ComputeTransforms(dt, speedN, distN, rotSpeedN, rotVectorN, scaleN, tmatN, rmatN, smatN);
	m_neptune->Update(tmatN* rmatN* smatN);

	ComputeTransforms(dt, speedPlu, distPlu, rotSpeedPlu, rotVectorPlu, scalePlu, tmatPlu, rmatPlu, smatPlu);
	m_pluto->Update(tmatPlu* rmatPlu* smatPlu);

	//modelStack.push();

	m_mesh->Update(dt);
	m_ship = m_mesh->GetModel();

	m_colS = m_sun->GetModel();
	m_colMe = m_mercury->GetModel();
	m_colV = m_venus->GetModel();
	m_colE = m_earth->GetModel();
	m_colMa = m_mars->GetModel();
	m_colJ = m_jupiter->GetModel();
	m_colSa = m_saturn->GetModel();
	m_colU = m_uranus->GetModel();
	m_colN = m_neptune->GetModel();
	m_colPlu = m_pluto->GetModel();

	sAngle = m_mesh->getAngle();
	glm::decompose(m_ship, scale, rotation, translation, skew, perspective);
	m_camera->Update(m_ship, translation, sAngle, m_colS, m_colMe, m_colV, m_colE, m_colMa, m_colJ, m_colSa, m_colU, m_colN, m_colPlu);
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

	float GlobalAmbient[3];
	float LightAmbient[3];
	float LightDiffuse[3];
	float LightSpecular[3];
	float LightPosition[3];

	for (int i = 0; i < 3; i++) {
		GlobalAmbient[i] = m_light->m_globalAmbient[i];
		LightAmbient[i] = m_light->m_lightAmbient[i];
		LightDiffuse[i] = m_light->m_lightDiffuse[i];
		LightSpecular[i] = m_light->m_lightSpecular[i];
		LightPosition[i] = m_light->m_lightPositionViewSpace[i];
	}

	m_light->SetViewSpacePosition(m_camera->GetView());
	
	/*GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);*/

	glProgramUniform4fv(m_shader->GetShaderProgram(), globalAmbientLocation, 1, GlobalAmbient);
	glProgramUniform4fv(m_shader->GetShaderProgram(), lightAmbientLocation, 1, LightAmbient);
	glProgramUniform4fv(m_shader->GetShaderProgram(), lightDiffuseLocation, 1, LightDiffuse);
	glProgramUniform4fv(m_shader->GetShaderProgram(), lightSpecularLocation, 1, LightSpecular);
	glProgramUniform3fv(m_shader->GetShaderProgram(), lightPositionLocation, 1, LightPosition);

	//Materials
	float MaterialAmbient[4] = {1.0, 1.0, 1.0, 1.0};
	float MaterialDiffuse[4] = { .5, .5, .5, 1.0 };
	float MaterialSpecular[4] = { 1.0, 1.0, 1.0, 1.0 };
	float MaterialShininess = 20.0;

	for (int i = 0; i < 4; i++) {
		MaterialAmbient[i] = m_light->m_lightAmbient[i];
		MaterialDiffuse[i] = m_light->m_lightDiffuse[i];
		MaterialSpecular[i] = m_light->m_lightSpecular[i];
	}
	/*
	for (int i = 0; i < 3; i++) {
		MaterialAmbient[i] = 1.0;
		MaterialSpecular[i] = 1.0;
		MaterialShininess = 20;
	}
	*/
	if (m_mesh != NULL) {
		glUniform1i(m_hasTexture, false);
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mesh->GetModel()));
		glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetView() * m_mesh->GetModel())))));
		// Material
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
		glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);

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

	/*for (int i = 0; i < 3; i++) {
		MaterialAmbient[i] = 1;
		MaterialSpecular[i] = .9;
		MaterialShininess = 50;
	}*/
	if (m_sun != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sun->GetModel()));
		glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetView() * m_sun->GetModel())))));

		// Material
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
		glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
		if (m_sun->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sun->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
		}
		if (m_sun->hasNorm) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_sun->getNormalID());
			GLuint sampler = m_shader->GetUniformLocation("samp1");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 1);
		}
		m_sun->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
	}

	for (int i = 0; i < 3; i++) {
		MaterialAmbient[i] = .3;
		MaterialDiffuse[i] = .5;
		MaterialSpecular[i] = .4;
		MaterialShininess = 10;
	}
	if (m_skybox != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_skybox->GetModel()));
		// Material
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
		glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
		if (m_skybox->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_skybox->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_skybox->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	
	
	MaterialSpecular[0] = 1;
	MaterialSpecular[1] = 1;
	MaterialSpecular[2] = 1;
	MaterialShininess = 20.0;
	for (int i = 0; i < 3; i++) {
		MaterialAmbient[i] = 1;
	}

	if (m_earth != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_earth->GetModel()));
		glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetView() * m_earth->GetModel())))));
		// Material
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
		glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
		if (m_earth->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_earth->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
		}
		if (m_earth->hasNorm) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_earth->getNormalID());
			GLuint sampler = m_shader->GetUniformLocation("samp1");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 1);
		}
		m_earth->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture, m_hasNormal);
	}


	// Render Moon
	if (m_luna != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_luna->GetModel()));
		// Material
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
		glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
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
		glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetView()* m_mercury->GetModel())))));
		// Material
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
		glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
		if (m_mercury->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mercury->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
		}
		if (m_mercury->hasNorm) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_mercury->getNormalID());
			GLuint sampler = m_shader->GetUniformLocation("samp1");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 1);
		}
		m_mercury->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
	}

	if (m_venus != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_venus->GetModel()));
		// Material
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
		glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
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
		// Material
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
		glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
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

	if (m_deimos != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_deimos->GetModel()));
		// Material
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
		glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
		if (m_deimos->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_deimos->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_deimos->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	if (m_phobos != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_phobos->GetModel()));
		// Material
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
		glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
		if (m_phobos->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_phobos->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_phobos->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}


	if (m_jupiter != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_jupiter->GetModel()));
		// Material
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
		glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
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

		if (m_ganymede != NULL) {
			glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ganymede->GetModel()));
			// Material
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
			glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
			if (m_ganymede->hasTex) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_ganymede->getTextureID());
				GLuint sampler = m_shader->GetUniformLocation("sp");
				if (sampler == INVALID_UNIFORM_LOCATION)
				{
					printf("Sampler Not found not found\n");
				}
				glUniform1i(sampler, 0);
				m_ganymede->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
			}
		}

		if (m_callisto != NULL) {
			glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_callisto->GetModel()));
			// Material
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
			glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
			if (m_callisto->hasTex) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_callisto->getTextureID());
				GLuint sampler = m_shader->GetUniformLocation("sp");
				if (sampler == INVALID_UNIFORM_LOCATION)
				{
					printf("Sampler Not found not found\n");
				}
				glUniform1i(sampler, 0);
				m_callisto->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
			}
		}

		if (m_europa != NULL) {
			glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_europa->GetModel()));
			// Material
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
			glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
			if (m_europa->hasTex) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_europa->getTextureID());
				GLuint sampler = m_shader->GetUniformLocation("sp");
				if (sampler == INVALID_UNIFORM_LOCATION)
				{
					printf("Sampler Not found not found\n");
				}
				glUniform1i(sampler, 0);
				m_europa->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
			}
		}

		if (m_io != NULL) {
			glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_io->GetModel()));
			// Material
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
			glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
			if (m_io->hasTex) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_io->getTextureID());
				GLuint sampler = m_shader->GetUniformLocation("sp");
				if (sampler == INVALID_UNIFORM_LOCATION)
				{
					printf("Sampler Not found not found\n");
				}
				glUniform1i(sampler, 0);
				m_io->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
			}
		}

	if (m_saturn != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_saturn->GetModel()));
		// Material
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
		glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
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

		if (m_ring != NULL) {
			glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ring->GetModel()));
			// Material
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
			glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
			if (m_ring->hasTex) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_ring->getTextureID());
				GLuint sampler = m_shader->GetUniformLocation("sp");
				if (sampler == INVALID_UNIFORM_LOCATION)
				{
					printf("Sampler Not found not found\n");
				}
				glUniform1i(sampler, 0);
				m_ring->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
			}
		}

		if (m_titan != NULL) {
			glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_titan->GetModel()));
			// Material
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
			glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
			if (m_titan->hasTex) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_titan->getTextureID());
				GLuint sampler = m_shader->GetUniformLocation("sp");
				if (sampler == INVALID_UNIFORM_LOCATION)
				{
					printf("Sampler Not found not found\n");
				}
				glUniform1i(sampler, 0);
				m_titan->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
			}
		}

		if (m_Smoon != NULL) {
			glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_Smoon->GetModel()));
			// Material
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
			glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
			glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
			if (m_Smoon->hasTex) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_Smoon->getTextureID());
				GLuint sampler = m_shader->GetUniformLocation("sp");
				if (sampler == INVALID_UNIFORM_LOCATION)
				{
					printf("Sampler Not found not found\n");
				}
				glUniform1i(sampler, 0);
				m_Smoon->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
			}
		}

	if (m_uranus != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_uranus->GetModel()));
		// Material
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
		glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
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
		// Material
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
		glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
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

	if (m_pluto != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_pluto->GetModel()));
		// Material
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialAmbientLocation, 1, MaterialAmbient);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialDiffuseLocation, 1, MaterialDiffuse);
		glProgramUniform4fv(m_shader->GetShaderProgram(), materialSpecularLocation, 1, MaterialSpecular);
		glProgramUniform1f(m_shader->GetShaderProgram(), materialShininessLocation, MaterialShininess);
		if (m_pluto->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_pluto->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_pluto->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Get any errors from OpenGL
	auto error = glGetError();
	if (error != GL_NO_ERROR)
	{
		string val = ErrorString(error);
		std::cout << "Error initializing OpenGL! " << error << ", " << val << std::endl;
	}

	//GLint globalAmbLoc = glGetUniformLocation(m_shader->GetShaderProgram(), "GlobalAmbient");
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

	// Locate the global ambient attribute
	globalAmbientLocation = m_shader->GetUniformLocation("GlobalAmbient");
	if (globalAmbientLocation == -1) {
		printf("globalAmbientLocation atribute not found\n");
		anyProblem = false;
	}

	lightAmbientLocation = m_shader->GetUniformLocation("light.ambient");
	if (lightAmbientLocation == -1) {
		printf("lightAmbientLocation atribute not found\n");
		anyProblem = false;
	}

	lightDiffuseLocation = m_shader->GetUniformLocation("light.diffuse");
	if (lightDiffuseLocation == -1) {
		printf("lightDiffuseLocation atribute not found\n");
		anyProblem = false;
	}

	lightSpecularLocation = m_shader->GetUniformLocation("light.spec");
	if (lightSpecularLocation == -1) {
		printf("lightSpecularLocation atribute not found\n");
		anyProblem = false;
	}

	lightPositionLocation = m_shader->GetUniformLocation("light.position");
	if (lightPositionLocation == -1) {
		printf("lightPositionLocation atribute not found\n");
		anyProblem = false;
	}

	materialAmbientLocation = m_shader->GetUniformLocation("material.ambient");
	if (materialAmbientLocation == -1) {
		printf("materialAmbientLocation atribute not found\n");
		anyProblem = false;
	}

	materialDiffuseLocation = m_shader->GetUniformLocation("material.diffuse");
	if (materialDiffuseLocation == -1) {
		printf("materialDiffuseLocation atribute not found\n");
		anyProblem = false;
	}

	materialSpecularLocation = m_shader->GetUniformLocation("material.spec");
	if (materialSpecularLocation == -1) {
		printf("materialSpecularLocation atribute not found\n");
		anyProblem = false;
	}

	materialShininessLocation = m_shader->GetUniformLocation("material.shininess");
	if (materialShininessLocation == -1) {
		printf("materialShininessLocation atribute not found\n");
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



