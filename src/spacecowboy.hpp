/**
 * @file spacecowboy.hpp
 *
 * Interface file for the spacecowboy class.
 */
#ifndef SPACE_COWBOY_SPACECOWBOY_HPP
#define SPACE_COWBOY_SPACECOWBOY_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

#include "objloader.hpp"
#include "camera.hpp"
#include "program.hpp"
#include "palette.hpp"

/**
* Represents a spacecowboy.
*/
class Spacecowboy {
public:
	// Constructors.
	/**
	 * Creates a stationary spacecowboy.
	 */
	Spacecowboy();

	// Accessor functions.
	/**
	 * Returns the position of the spacecowboy.
	 *
	 * @return Position of spacecowboy.
	 */
	glm::vec3 position() const;

	/**
	 * Returns the velocity of the spacecowboy.
	 *
	 * @return Velocity of spacecowboy.
	 */
	glm::vec3 velocity() const;

	// Mutator functions.
	/**
	 * Sets the velocity of the spacecowboy.
	 *
	 * @param velocity New spacecowboy velocity.
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
	 * Sets the opacity of the spacecowboy.
	 *
	 * @param alpha Alpha value of opacity.
	 */
	void setOpacity(GLfloat alpha) const;

	/**
	 * Translates the spacecowboy in space.
	 *
	 * @param displacement spacecowboy displacement.
	 */
	void translate(const glm::vec3& displacement);

	/**
	 * Sets the spacecowboy's position.
	 *
	 * @param position spacecowboy's new position.
	 */
	void setPosition(const glm::vec3& position);

	/**
	 * Scales the spacecowboy uniformly.
	 *
	 * @param scaleFactor Scale factor.
	 */
	void scale(float scaleFactor);

	/**
	 * Updates the spacecowboy state based on its state of motion.
	 */
	void updateState(const Camera& camera);
	/**
	 * Renders the spacecowboy.
	 *
	 * @param camera Camera object used to render the spacecowboy.
	 */
	void draw(const Camera& camera) const;

	/**
	 * Returns the model matrix of the spacecowboy.
	 *
	 * @return spacecowboy's model matrix.
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

	/** Reference ID of vertex uv buffer. */
	GLuint  mUV_VBO;
	/**Reference ID of vertex normal buffer*/
	GLuint mN_VBO;

	/**Reference ID of ship texture*/
	GLuint spacecowboy_texture;

	/** Number of vertices in the spacecowboy's mesh data. */
	unsigned int mNVertices;
	/** Number of indices in the spacecowboy's element buffer. */
	unsigned int mNIndices;

	/** Scale matrix. */
	glm::mat4 mScale;
	/** Rotation matrix. */
	glm::mat4 mRotation;
	/** Translation matrix. */
	glm::mat4 mTranslation;


	/** Position of the spacecowboy. */
	glm::vec3 mPosition;
	/** Velocity of the spacecowboy. */
	glm::vec3 mVelocity;
	/** Time in seconds of last frame. */
	double    mTimeLastFrame;
};

#endif
