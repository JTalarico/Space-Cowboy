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
	* Copy constructor is disabled as OpenGL does not permit the shallow copying of buffer objects.
	*/
	Sun(const Sun& other) = delete;

	/**
	* Copy assignment operator is disabled as OpenGL does not permit the shallow copying of buffer
	* objects.
	*/
	Sun& operator=(const Sun& other) = delete;

	// Destructors.
	/**
	* Destroys the vertex array object, vertex buffer object, and element buffer object.
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

	bool sunCollision(const Camera &camera);

public:
	// Data members.
	/** Shader program. */
	Program mProgram;

	/** Scale matrix. */
	glm::mat4 mScale;

	/** Reference ID of vertex array buffer. */
	GLuint sVAO;
	/** Reference ID of vertex buffer object. */
	GLuint sVBO;
	/** Reference ID of element buffer object. */
	GLuint sEBO;

	GLuint mUV_VBO;

	GLuint sun_texture;

	/** Number of vertices in the sun's mesh data. */
	unsigned int mNVertices;
	/** Number of indices in the sun's element buffer. */
	unsigned int mNIndices;
};

#endif

