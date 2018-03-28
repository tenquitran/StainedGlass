#pragma once

#include "TextureProjector.h"


namespace StainedGlassApp
{
	// OpenGL scene.
	class Scene
	{
	public:
		Scene();

		virtual ~Scene();

		// Parameters: aspectRatio - aspect ratio of the window's client area;
		//             openGlInfo - OpenGL settings.
		bool initialize(GLfloat aspectRatio, const CommonLibOgl::OpenGLInfo& openGlInfo);

		// Update Model-View-Projection (MVP) and other matrices in the GLSL program.
		void updateViewMatrices() const;

		void resize(GLfloat aspectRatio);

		void render() const;

		//////////////////////////////////////////////////////////////////////////
		// Camera control.
		//////////////////////////////////////////////////////////////////////////

		void translateCameraX(GLfloat diff);
		void translateCameraY(GLfloat diff);
		void translateCameraZ(GLfloat diff);

		void rotateCameraX(GLfloat angleDegrees);
		void rotateCameraY(GLfloat angleDegrees);
		void rotateCameraZ(GLfloat angleDegrees);

		void rotateCameraXY(GLfloat xAngleDegrees, GLfloat yAngleDegrees);

		GLfloat getCameraScale() const;

		void scaleCamera(GLfloat amount);

	private:
		std::unique_ptr<CommonLibOgl::ProgramGLSL> m_spProgram;

		// OpenGL camera.
		std::unique_ptr<CommonLibOgl::Camera> m_spCamera;

#if 1
		// Horizontal plane.

		GLuint m_vaoPlane;

		GLuint m_vboPlane;

		GLuint m_indexPlane;          // index buffer
		GLsizei m_indexCountPlane;    // number of indices

		GLuint m_normalPlane;         // normal buffer

		GLuint m_colorPlane;          // color buffer
#endif

		// Projected texture;
		GLuint m_textureGlass;

#if 1
		std::unique_ptr<TextureProjector> m_spTextureProjector;
#endif
	};
}
