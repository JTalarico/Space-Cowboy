/**
 * @file planet.hpp
 *
 * Interface file for the Planet class.
 */
#ifndef SPACE_COWBOY_PLANET_HPP
#define SPACE_COWBOY_PLANET_HPP

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
 * Represents a planet.
 */
class Planet {
public:
	// Constructors.
	/**
	 * Creates a unit sphere centred at the origin.
	 */
	Planet();

	/**
	 * Copy constructor is disabled as OpenGL does not permit the shallow copying of buffer objects.
	 */
	Planet(const Planet& other) = delete;

	/**
	 * Copy assignment operator is disabled as OpenGL does not permit the shallow copying of buffer
	 * objects.
	 */
	Planet& operator=(const Planet& other) = delete;

	// Destructors.
	/**
	 * Destroys the vertex array object, vertex buffer object, and element buffer object.
	 */
	~Planet();

	// Accessor functions.
	/**
	 * Returns the planet's angular velocity.
	 *
	 * @return Planet's angular velocity.
	 */
	glm::vec3 angularVelocity() const;

	/**
	 * Returns the model matrix of the planet.
	 *
	 * @return Planet's model matrix.
	 */
	glm::mat4 modelMatrix() const;

	/**
	 * Returns the planet's orbital angular velocity.
	 *
	 * @return Planet's orbital angular velocity.
	 */
	glm::vec3 orbitalAngularVelocity() const;

	/**
	 * Returns the planet's position.
	 *
	 * @return Planet's position.
	 */
	glm::vec3 position() const;

	// Planet modifier functions.
	/**
	 * Rotates the planet.
	 *
	 * @param angle Angle to rotate by, in radians.
	 * @param rotationAxis Axis to rotate around.
	 */
	void rotate(float angle, const glm::vec3& rotationAxis);

	/**
	 * Rotates the planet.
	 *
	 * @param angle Angle to rotate by, in radians.
	 * @param nx x component of rotation axis.
	 * @param ny y component of rotation axis.
	 * @param nz z component of rotation axis.
	 */
	void rotate(float angle, float nx, float ny, float nz);

	/**
	 * Rotates the planet around the origin.
	 *
	 * @param angle Angle to rotate by, in radians.
	 * @param rotationAxis Axis to rotate around.
	 */
	void rotateAroundOrigin(float angle, const glm::vec3& rotationAxis);

	/**
	 * Rotates the planet around the origin.
	 *
	 * @param angle Angle to rotate by, in radians.
	 * @param nx x component of rotation axis.
	 * @param ny y component of rotation axis.
	 * @param nz z component of rotation axis.
	 */
	void rotateAroundOrigin(float angle, float nx, float ny, float nz);

	/**
	 * Scales the planet uniformly.
	 *
	 * @param scaleFactor Scale factor.
	 */
	void scale(float scaleFactor);

	/**
	 * Sets the planet's angular velocity.
	 *
	 * @param angularVelocity Planet's new angular velocity.
	 */
	void setAngularVelocity(const glm::vec3& angularVelocity);

	/**
	 * Sets the planet's angular velocity.
	 *
	 * @param angularVelocity_x x component of planet's new angular velocity.
	 * @param angularVelocity_y y component of planet's new angular velocity.
	 * @param angularVelocity_z z component of planet's new angular velocity.
	 */
	void setAngularVelocity(float angularVelocity_x, float angularVelocity_y,
	                        float angularVelocity_z);

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

	/**
	 * Sets the planet's orbital angular velocity about the world's origin.
	 *
	 * @param orbitalAngularVelocity Planet's new orbital angular velocity.
	 */
	void setOrbitalAngularVelocity(const glm::vec3& orbitalAngularVelocity);

	/**
	 * Sets the planet's orbital angular velocity about the world's origin.
     *
	 * @param orbitalAV_x x component of planet's new orbital angular velocity.
	 * @param orbitalAV_y y component of planet's new orbital angular velocity.
	 * @param orbitalAV_z z component of planet's new orbital angular velocity.
	 */
	void setOrbitalAngularVelocity(float orbitalAV_x, float orbitalAV_y, float orbitalAV_z);

	/**
	 * Translates the planet in space.
	 *
	 * @param displacement Cube displacement.
	 */
	void translate(const glm::vec3& displacement);

	/**
	 * Translates the planet in space.
	 *
	 * @param x Displacement in x direction.
	 * @param y Displacement in y direction.
	 * @param z Displacement in z direction.
	 */
	void translate(GLfloat x, GLfloat y, GLfloat z);

	/**
	 * Update's the planet's state based on its state of motion.
	 */
	void updateState();

	// OpenGL modifier functions.
	/**
	 * Renders the planet.
	 *
	 * @param camera Camera object used to render the planet.
	 */
	void draw(const Camera& camera) const;

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

	/** Planet's angular velocity about its own centre. */
	glm::vec3 mAngularVelocity;
	/** Planet's orbital angular velocity about the centre of the world. */
	glm::vec3 mOrbitalAngularVelocity;
	/** Time in seconds since last state update. */
	double    mTimeLastStateUpdate;
};

#endif
