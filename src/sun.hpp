/**
 * @file sun.hpp
 *
 * Interface file for the Sun class.
 */
#ifndef SPACE_COWBOY_SUN_HPP
#define SPACE_COWBOY_SUN_HPP

#include "camera.hpp"
#include "palette.hpp"
#include "program.hpp"
#include "sphere.hpp"

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * Represents a Sun. Serves as a central source of illumination for the solar system. Note that the
 * camera used to render the sun must be with the function setCamera, or else the sun will not
 * render.
 */
class Sun {
public:
	// Constructors.
	/**
	 * Constructs a sphere of unit size centred at the origin.
	 */
	Sun();

	/**
	 * Copies a sun's transformation matrices but creates a unique shader program.
	 */
	Sun(const Sun& other);

	/**
	 * Copies a sun's transformation matrices.
	 */
	Sun& operator=(const Sun& other);

	// Destructors.
	/**
	 * Destroys the vertex array object, vertex buffer object, and element buffer object if there
	 * are no more suns remaining.
	 */
	~Sun();

	// Accessor functions.
	/**
	 * Returns the model matrix for the sun.
	 *
	 * @return Sun's model matrix.
	 */
	glm::mat4 modelMatrix() const;

	// Mutator functions.
	/**
	 * Scale the sun.
	 *
	 * @param scaleFactor Scale factor.
	 */
	void scale(float scaleFactor);

	/**
	 * Sets the colour of the Planet in RGB colour space.
	 *
	 * @param r Red value. Should be between 0.0 and 1.0.
	 * @param g Green value. Should be between 0.0 and 1.0.
	 * @param b Blue value. Should be between 0.0 and 1.0.
	 */
	void setColour(GLfloat r, GLfloat g, GLfloat b) const;

	/**
	 * Sets the colour of the Planet in RGB colour space.
	 *
	 * @param colour RGB values.
	 */
	void setColour(const palette::rgb_t& colour) const;

	/**
	 * Sets the opacity of the planet.
	 *
	 * @param alpha Alpha value of opacity.
	 */
	void setOpacity(GLfloat alpha) const;

	// OpenGL modifiers.
	/**
	 * Renders the sun.
	 *
	 * @param camera Camera object used to render the sun.
	 */
	void draw(const Camera& camera) const;

public:
	// Data members.
	/** Shader program. */
	Program mProgram;

	/** Scale matrix. */
	glm::mat4 mScale;

	// Static data members.
	/** Number of suns that have been created. */
	static int sNSuns;

	/** Reference ID of vertex array buffer. */
	static GLuint sVAO;
	/** Reference ID of vertex buffer object. */
	static GLuint sVBO;
	/** Reference ID of element buffer object. */
	static GLuint sEBO;

	/** Sun vertices. */
	static std::vector<GLfloat> sVertices;
	/** Order in which to draw the vertices. */
	static std::vector<GLuint>  sIndices;
};

#endif
