#pragma once


namespace StainedGlassApp
{
	// Entity to project texture on the scene.
	class TextureProjector
	{
	public:
		TextureProjector(GLuint program, const CommonLibOgl::Camera& camera);

		virtual ~TextureProjector();

		void translateX(GLfloat diff);
		void translateY(GLfloat diff);
		void translateZ(GLfloat diff);

		void scale(GLfloat amount);

	private:
		TextureProjector(const TextureProjector&) = delete;
		TextureProjector& operator=(const TextureProjector&) = delete;

		void updateMatrix() const;

	private:
		const GLuint m_program;

		const CommonLibOgl::Camera& m_camera;

		const glm::vec3 m_position;
		
		const glm::vec3 m_lookAt;
		
		const glm::vec3 m_upVector;

		glm::vec3 m_translation;

		// Minimum scale factor.
		const GLfloat ScaleFactorMin;

		GLfloat m_scaleFactor;
	};
}
