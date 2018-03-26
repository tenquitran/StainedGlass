#pragma once


namespace StainedGlassApp
{
	// Entity to project texture to the scene.
	class TextureProjector
	{
	public:
		explicit TextureProjector(GLuint program);

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

		const glm::vec3 m_position;
		
		const glm::vec3 m_direction;
		
		const glm::vec3 m_upVector;

		glm::vec3 m_translation;

#if 0
		// Initial scale factor.
		const GLfloat ScaleFactorInit;
#endif

		// Minimum scale factor.
		const GLfloat ScaleFactorMin;

		GLfloat m_scaleFactor;
	};
}
