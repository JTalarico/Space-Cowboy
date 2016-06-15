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
#include <cmath>

/**
 * Represents a planet.
 */
class Planet {
public:
	// Enumerated texture types
	enum TERRAIN_TYPE {
		ROCKY, GASEOUS, EARTH_LIKE
	};

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

	/**
     * Returns the planet's size.
     *
     * @return Planet's size.
     */
	float size() const;

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

	bool planetCollision(const Camera& camera);

	/**
	 * Changes the planet texture based on an enumerated type.
	 * Then updates the VBO
	 */
	void setPlanetTextureType(TERRAIN_TYPE terrain);

	/**
	 * Changes the planet texture to rocky.
	 * Color VBO must be updated immediately after.
	 */
	void setRockyTexture();

	/**
	 * Changes the planet texture to gaseous.
	 * Color VBO must be updated immediately after.
	 */
	void setGaseousTexture();

	/**
	 * Changes the planet texture to earth like.
	 * Color VBO must be updated immediately after.
	 */
	void setEarthLikeTexture();

	/**
	 * Generates the texture of the planet based on input parameters
	 * Color VBO must be updated immediately after.
	 */
	void generateTexture(glm::vec3 brightColor, glm::vec3 darkColor, double xPeriod, double yPeriod,
	                     double turbPower, double trubSize);

protected:
	// Data members.
	/** Shader program. */
	Program mProgram;
	/** Reference ID of vertex array buffer. */
	GLuint  mVAO;
	/** Reference ID of vertex buffer object. */
	GLuint  mVBO;
	/** Reference ID of vertex buffer object. */
	GLuint  mColorVBO;
	/** Reference ID of element buffer object. */
	GLuint  mEBO;

	/** Number of vertices in the planet's mesh data. */
	unsigned int mNVertices;
	/** Number of indices in the planet's element buffer. */
	unsigned int mNIndices;
	/** Number of colors in the planet's array buffer. */
	unsigned int mNColors;

	/** Color of each vertex in the sphere */
	std::vector<GLfloat> mVertexColors;

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

	// Functions
	/** Update buffers related to the planet */
	void updateBuffers();
};

float                             smoothNoise(float x, float y,
                                              const std::vector<std::vector<GLfloat>>& noise);

std::vector<std::vector<GLfloat>> generateNoiseMatrix();

double                            turbulence(double x, double y, double size,
                                             const std::vector<std::vector<GLfloat>>& noise);


glm::vec3 getBrightColor();

glm::vec3 getDarkColor();

#endif

