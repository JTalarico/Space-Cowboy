/**
 * @file stars.hpp
 *
 * Interface file for the Stars class.
 */
#ifndef SPACE_COWBOY_STARS_HPP
#define SPACE_COWBOY_STARS_HPP

#include "camera.hpp"
#include "program.hpp"

#include <array>
#include <stdexcept>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

/**
 * Produces a procedurally generated skybox depicting many stars.
 */
class Stars {
public:
	// Constructors.
	/**
	 * Loads the texture files for the skybox.
	 */
	Stars();

	// OpenGL modifier functions.
	/**
	 * Renders the stars.
	 *
	 * @param camera Camera used to render the stars.
	 */
	void draw(const Camera& camera) const;

private:
	// Data members.
	/** Reference ID for the texture. */
	GLuint mTextureID;

	/** Shader program. */
	Program mProgram;
	/** Reference ID of vertex array buffer. */
	GLuint  mVAO;
	/** Reference ID of vertex buffer object. */
	GLuint  mVBO;

	// Static data members.
	/** Skybox vertices. */
	//@formatter:off
	static constexpr std::array<GLfloat, 108> sVertices = {
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
	};
	//@formatter:on

	// Helper functions.
	/**
	 * Creates a texture for a cube from the appropriate image files and returns the reference ID to
	 * the texture. Throws std::runtime_error if image files cannot be found.
	 */
	static GLuint createTexture();
};

#endif
