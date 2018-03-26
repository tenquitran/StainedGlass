#include "stdafx.h"
#include "TextureProjector.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;
using namespace StainedGlassApp;

//////////////////////////////////////////////////////////////////////////


TextureProjector::TextureProjector(GLuint program)
	: m_program(program), m_position(-15.0f, 5.0f, -7.5f), m_direction(15.0f, 5.0f, -7.5f), 
	  m_upVector(0.0f, 1.0f, 0.0f), /*ScaleFactorInit(1.0f),*/ ScaleFactorMin(0.01f), m_scaleFactor(1.0f)
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
#if 0
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

	static const glm::mat4 view = glm::lookAt(m_position, m_direction, m_upVector);

	static const glm::mat4 proj = glm::perspective(30.0f, 1.0f, 0.2f, 1000.0f);

	glm::mat4 scaleTranslate = glm::translate(glm::mat4(1.0f), m_translation) * 
		//glm::scale(glm::mat4(1.0f), glm::vec3(ScaleFactorInit)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(m_scaleFactor));

#if 0
	// Apply the initial scale factor.
	m_model *= glm::scale(glm::mat4(1.0f), glm::vec3(SCALE_FACTOR_INITIAL, SCALE_FACTOR_INITIAL, SCALE_FACTOR_INITIAL));

	// Apply the user-defined scale factor.
	m_model *= glm::scale(glm::mat4(1.0f), glm::vec3(m_scaleFactorVariable, m_scaleFactorVariable, m_scaleFactorVariable));
#endif

	glm::mat4 texProjectorMatrix = scaleTranslate * proj * view;

	glUseProgram(m_program);

	glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(texProjectorMatrix));

	glUseProgram(0);
}
