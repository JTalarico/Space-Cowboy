/**
 * @file spaceship.hpp
 *
 * Interface file for the Spaceship class.
 */
#ifndef SPACE_COWBOY_SPACESHIP_HPP
#define SPACE_COWBOY_SPACESHIP_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "objloader.hpp"
#include "camera.hpp"
#include "program.hpp"
#include "palette.hpp"

/**
 * Represents a user-controlled spaceship.
 */
class Spaceship {
public:
	// Constructors.
	/**
	 * Creates a stationary spaceship centred at the origin.
	 */
	Spaceship();

	// Accessor functions.
	/**
	 * Returns the position of the spaceship.
	 *
	 * @return Position of spaceship.
	 */
	glm::vec3 position() const;

	/**
	 * Returns the velocity of the spaceship.
	 *
	 * @return Velocity of spaceship.
	 */
	glm::vec3 velocity() const;

	// Mutator functions.
	/**
	 * Sets the velocity of the spaceship.
	 *
	 * @param velocity New spaceship velocity.
	 */
	void setVelocity(const glm::vec3& velocity);

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
	* Sets the opacity of the spaceship.
	*
	* @param alpha Alpha value of opacity.
	*/
	void setOpacity(GLfloat alpha) const;

	/**
	 * Translates the spaceship in space.
	 *
	 * @param displacement Spaceship displacement.
	 */

	void translate(const glm::vec3& displacement);

	/**
	* Sets the spaceship's position.
	*
	* @param position spaceships's new position.
	*/
	void setPosition(const glm::vec3& position);

	/**
	* Scales the spaceship uniformly.
	*
	* @param scaleFactor Scale factor.
	*/
	void scale(float scaleFactor);

	/**
	 * Updates the spaceships state based on its state of motion.
	 */
	void updateState();
	/**
	* Renders the spaceship.
	*
	* @param camera Camera object used to render the spaceship.
	*/
	void draw(const Camera& camera) const;

	/**
	* Returns the model matrix of the spaceship.
	*
	* @return Spaceship's model matrix.
	*/
	glm::mat4 modelMatrix() const;

private:
	// Data members.

	/** Shader program. */
	Program mProgram;
	/** Reference ID of vertex array buffer. */
	GLuint  mVAO;
	/** Reference ID of vertex buffer object. */
	GLuint  mVBO;
	/** Reference ID of element buffer object. */
	GLuint  mEBO;

	/** Reference ID of vertex uv buffer. */
	GLuint  mUV_VBO;

	/** Number of vertices in the planet's mesh data. */
	unsigned int mNVertices;
	/** Number of indices in the planet's element buffer. */
	unsigned int mNIndices;

	/** Scale matrix. */
	glm::mat4 mScale;
	/** Rotation matrix. */
	glm::mat4 mRotation;
	/** Translation matrix. */
	glm::mat4 mTranslation;


	/** Position of the spaceship. */
	glm::vec3 mPosition;
	/** Velocity of the spaceship. */
	glm::vec3 mVelocity;
	/** Time in seconds of last frame. */
	double    mTimeLastFrame;

	
	

};

#endif
