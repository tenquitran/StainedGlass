#include "stdafx.h"
#include "TextureProjector.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;
using namespace StainedGlassApp;

//////////////////////////////////////////////////////////////////////////


TextureProjector::TextureProjector(GLuint program, const Camera& camera)
	: m_program(program), m_camera(camera), m_position(/*-25.0f, 5.0f, -7.5f*/ -15.0f, 5.0f, 0.0f /*0.0f, 5.0f, 0.0f // ceiling */),
	m_lookAt(/*0.0f, -4.0f, -12.5f*/ /*15.0f, -4.0f, -12.5f*/  0.0f, -4.0f, 0.0f /* 0.0f, -1.0f, 0.0f // ceiling  */),
	m_upVector(0.0f, 1.0f, 0.0f  /*0.0f, 0.0f, 1.0f // ceiling  */), 
	ScaleFactorMin(0.01f), m_scaleFactor(1.0f)
{
	if (0 == m_program)
	{
		throw EXCEPTION(L"Invalid program ID");
	}

	updateMatrix();
}

TextureProjector::~TextureProjector()
{
}

void TextureProjector::translateX(GLfloat diff)
{
	m_translation.x += diff;

	updateMatrix();
}

void TextureProjector::translateY(GLfloat diff)
{
	m_translation.y += diff;

	updateMatrix();
}

void TextureProjector::translateZ(GLfloat diff)
{
	m_translation.z += diff;

	updateMatrix();
}

void TextureProjector::scale(GLfloat amount)
{
	m_scaleFactor += amount;

	// Prevent the projector flipping from zooming in to zooming out when the scale factor changes sign.
	if (m_scaleFactor < 0)
	{
		m_scaleFactor = ScaleFactorMin;
		//OutputDebugStringW(L"Texture projector: ZERO\n");
	}

	updateMatrix();
}

void TextureProjector::updateMatrix() const
{
	static const glm::mat4 view = glm::lookAt(m_position, m_lookAt, m_upVector);

	static const glm::mat4 projection = glm::perspective(glm::radians(30.0f), 1.0f, 0.2f, 1000.0f);

	glm::mat4 bias = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f)) * 
					 glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

	glm::mat4 texProjectorMatrix = bias * projection * view * glm::inverse(m_camera.getViewMatrix());

	glUseProgram(m_program);

	glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(texProjectorMatrix));

	glUseProgram(0);
}
