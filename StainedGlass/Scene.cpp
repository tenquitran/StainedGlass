#include "stdafx.h"
#include "Scene.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;
using namespace StainedGlassApp;

//////////////////////////////////////////////////////////////////////////


Scene::Scene()
	: m_vaoPlane{}, m_vboPlane{}, m_indexPlane{}, m_indexCountPlane{}, m_normalPlane{}, m_colorPlane{}, m_textureGlass{}
{
}

Scene::~Scene()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (0 != m_textureGlass)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &m_textureGlass);
	}

	if (0 != m_colorPlane)
	{
		glDeleteBuffers(1, &m_colorPlane);
	}

	if (0 != m_normalPlane)
	{
		glDeleteBuffers(1, &m_normalPlane);
	}

	if (0 != m_indexPlane)
	{
		glDeleteBuffers(1, &m_indexPlane);
	}

	if (0 != m_vboPlane)
	{
		glDeleteBuffers(1, &m_vboPlane);
	}

	if (0 != m_vaoPlane)
	{
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_vaoPlane);
	}
}

bool Scene::initialize(GLfloat aspectRatio, const OpenGLInfo& openGlInfo)
{
	// Initial scale factor for the camera.
	const GLfloat CameraScaleFactor = 1.0f;

	m_spCamera = std::make_unique<Camera>(aspectRatio, CameraScaleFactor,
		openGlInfo.FieldOfView, openGlInfo.FrustumNear, openGlInfo.FrustumFar);

	m_spCamera->translateZ(-20.0f);
	//m_spCamera->scale(0.05f);
	//m_spCamera->translateZ(-20.0f);
	//m_spCamera->rotateX(25.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_MULTISAMPLE);

	glClearColor(0.8f, 0.93f, 0.96f, 1.0f);    // very light blue

	// Initialize the program wrapper.

	const ShaderCollection shaders = {
		{ GL_VERTEX_SHADER,   "shaders\\stainedGlass.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\stainedGlass.frag" }
	};

	m_spProgram = std::make_unique<ProgramGLSL>(shaders);

	// Generate data for the horizontal plane.
#if 1
	glGenVertexArrays(1, &m_vaoPlane);
	glBindVertexArray(m_vaoPlane);

	// Set up the vertex buffer.

	float vertices[] = {
		-15.0f, -4.0f,   0.0f,
		 15.0f, -4.0f,   0.0f,
		-15.0f, -4.0f, -15.0f,
		 15.0f, -4.0f, -15.0f };

	glGenBuffers(1, &m_vboPlane);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboPlane);
	glBufferData(GL_ARRAY_BUFFER, _countof(vertices) * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

	// Fill in the vertex position attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Set up the index buffer.

	GLuint indices[] = {
		0, 1, 3,
		0, 3, 2 };

	m_indexCountPlane = _countof(indices);

	glGenBuffers(1, &m_indexPlane);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexPlane);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _countof(indices) * sizeof(indices[0]), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Set up the normal buffer.

	GLfloat normals[] = {
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glGenBuffers(1, &m_normalPlane);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalPlane);
	glBufferData(GL_ARRAY_BUFFER, _countof(normals) * sizeof(normals[0]), normals, GL_STATIC_DRAW);

	// Fill in the normal attribute.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);

	// Set up the color buffer.

	GLfloat colors[] = {
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f
	};

	glGenBuffers(1, &m_colorPlane);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorPlane);
	glBufferData(GL_ARRAY_BUFFER, _countof(colors) * sizeof(colors[0]), colors, GL_STATIC_DRAW);

	// Fill in the color attribute.
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(2);
#endif

	// Create stained glass texture and set its parameters.

	glGenTextures(1, &m_textureGlass);
	glBindTexture(GL_TEXTURE_2D, m_textureGlass);

	int width = {}, height = {};

	auto del = [](unsigned char* pBuff) {
		SOIL_free_image_data(pBuff);
	};

	//std::string filePath = "data//test.png";
	std::string filePath = "data//Moorish_design.png";

	std::unique_ptr<unsigned char[], decltype(del)> spData(
		SOIL_load_image(filePath.c_str(), &width, &height, 0, SOIL_LOAD_RGB));
	if (!spData)
	{
		// We can check unique_ptr for NULL because of SOIL_load_image() behaviour.
		std::wcerr << L"Failed to load texture " << filePath.c_str() << L": " << SOIL_last_result() << '\n';
		assert(false); return false;
	}

	// Copy the image data to the texture object.
	glBindTexture(GL_TEXTURE_2D, m_textureGlass);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, spData.get());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// Material for the plane.
	MaterialPhong materialPlane({ 0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, 0.5f }, 32.0f);

	// Light source properties.
	glm::vec3 ambient(0.15f, 0.15f, 0.15f);
	glm::vec3 diffuse(0.6f, 0.6f, 0.6f);
	glm::vec3 specular(0.25f, 0.25f, 0.25f);
	glm::vec4 lightPos(0.0f, 5.0f, 0.0f, 0.0f);

#if 1
	// Create texture projector.
	m_spTextureProjector = std::make_unique<TextureProjector>(m_spProgram->getProgram(), *m_spCamera);
#else
	// "Texture projector" properties.
	glm::vec3 projPos(-15.0f, 5.0f, -7.5f);
	//glm::vec3 projPos(2.0f, 5.0f, 5.0f);
	glm::vec3 projAt(15.0f, 5.0f, -7.5f);
	//glm::vec3 projAt(-2.0f, -4.0f, 0.0f);
	glm::vec3 projUp(0.0f, 1.0f, 0.0f);
	glm::mat4 projView = glm::lookAt(projPos, projAt, projUp);
	glm::mat4 projProj = glm::perspective(30.0f, 1.0f, 0.2f, 1000.0f);
	glm::mat4 projScaleTrans = glm::mat4(1.0f);
	//glm::mat4 projScaleTrans = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f));
	glm::mat4 texProjectorMatrix = projScaleTrans * projProj * projView;
#endif

	GLuint program = m_spProgram->getProgram();

	glUseProgram(program);

	// Set material properties.

	GLuint mtAmbient = glGetUniformLocation(program, "Material.ambient");
	if (-1 != mtAmbient)
	{
		glUniform3fv(mtAmbient, 1, glm::value_ptr(materialPlane.m_ambientColor));
	}

	GLuint mtDiffuse = glGetUniformLocation(program, "Material.diffuse");
	if (-1 != mtDiffuse)
	{
		glUniform3fv(mtDiffuse, 1, glm::value_ptr(materialPlane.m_diffuseColor));
	}

	GLuint mtSpecular = glGetUniformLocation(program, "Material.specular");
	if (-1 != mtSpecular)
	{
		glUniform3fv(mtSpecular, 1, glm::value_ptr(materialPlane.m_specularColor));
	}

	GLuint mtShininess = glGetUniformLocation(program, "Material.shininess");
	if (-1 != mtShininess)
	{
		glUniform1f(mtShininess, materialPlane.m_shininess);
	}

	// Set light properties.

#if 1
	GLuint ltAmbient = glGetUniformLocation(program, "Light.ambient");
	if (-1 != ltAmbient)
	{
		glUniform3fv(ltAmbient, 1, glm::value_ptr(ambient));
	}
#endif

	GLuint ltDiffuse = glGetUniformLocation(program, "Light.diffuse");
	if (-1 != ltDiffuse)
	{
		glUniform3fv(ltDiffuse, 1, glm::value_ptr(diffuse));
	}

	GLuint ltSpecular = glGetUniformLocation(program, "Light.specular");
	if (-1 != ltSpecular)
	{
		glUniform3fv(ltSpecular, 1, glm::value_ptr(specular));
	}

	GLuint ltPosition = glGetUniformLocation(program, "Light.position");
	if (-1 != ltPosition)
	{
		glUniform4fv(ltPosition, 1, glm::value_ptr(lightPos));
	}

#if 0
	// Texture projector matrix.
	glUniformMatrix4fv(6, 1, GL_FALSE, glm::value_ptr(texProjectorMatrix));
#endif

	// Stained glass texture sampler.
	GLuint texSampler = glGetUniformLocation(program, "glassTexture");
	if (-1 != texSampler)
	{
		glUniform1i(texSampler, 0);
	}

	glUseProgram(0);

	if (!m_spProgram->validate())
	{
		std::wcerr << L"GLSL program validation failed\n";
		assert(false); return false;
	}

	updateViewMatrices();

	return true;
}

void Scene::updateViewMatrices() const
{
	assert(m_spProgram->getProgram());

	glUseProgram(m_spProgram->getProgram());

	// Plane.
#if 1
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(m_spCamera->getModelViewProjectionMatrix()));

	glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(m_spCamera->getModelMatrix()));

	glm::mat4 mv = m_spCamera->getModelViewMatrix();

	glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(mv));

	glm::mat3 normal = glm::mat3(glm::transpose(glm::inverse(mv)));
	//glm::mat3 normal = glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2]));

	glUniformMatrix3fv(3, 1, GL_FALSE, glm::value_ptr(normal));

	//glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(m_spCamera->getProjectionMatrix()));

	//glm::vec3 cameraPos = m_spCamera->getTranslation();

	//glUniform3fv(5, 1, glm::value_ptr(cameraPos));
#endif

	glUseProgram(0);
}

void Scene::translateCameraX(GLfloat diff)
{
	m_spCamera->translateX(diff);
	m_spTextureProjector->translateX(diff);

	updateViewMatrices();
}

void Scene::translateCameraY(GLfloat diff)
{
	m_spCamera->translateY(diff);
	m_spTextureProjector->translateY(diff);

	updateViewMatrices();
}

void Scene::translateCameraZ(GLfloat diff)
{
	m_spCamera->translateZ(diff);
	m_spTextureProjector->translateZ(diff);

	updateViewMatrices();
}

void Scene::rotateCameraX(GLfloat angleDegrees)
{
	m_spCamera->rotateX(angleDegrees);

	updateViewMatrices();
}

void Scene::rotateCameraY(GLfloat angleDegrees)
{
	m_spCamera->rotateY(angleDegrees);

	updateViewMatrices();
}

void Scene::rotateCameraZ(GLfloat angleDegrees)
{
	m_spCamera->rotateZ(angleDegrees);

	updateViewMatrices();
}

void Scene::rotateCameraXY(GLfloat xAngleDegrees, GLfloat yAngleDegrees)
{
	m_spCamera->rotateXY(xAngleDegrees, yAngleDegrees);

	updateViewMatrices();
}

GLfloat Scene::getCameraScale() const
{
	return m_spCamera->getScale();
}

void Scene::scaleCamera(GLfloat amount)
{
	m_spCamera->scale(amount);
	m_spTextureProjector->scale(amount);

	updateViewMatrices();
}

void Scene::resize(GLfloat aspectRatio)
{
	m_spCamera->resize(aspectRatio);

	updateViewMatrices();
}

void Scene::render() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render the plane.
	GLuint program = m_spProgram->getProgram();

#if 1
	assert(program);

	glUseProgram(program);
	glBindVertexArray(m_vaoPlane);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboPlane);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexPlane);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureGlass);

	glDrawElements(GL_TRIANGLES, m_indexCountPlane, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
#endif
}
