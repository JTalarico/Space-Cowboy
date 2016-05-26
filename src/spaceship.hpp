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
	 * Translates the spaceship in space.
	 *
	 * @param displacement Spaceship displacement.
	 */
	void translate(const glm::vec3& displacement);

	/**
	 * Updates the spaceships state based on its state of motion.
	 */
	void updateState();

private:
	/** Position of the spaceship. */
	glm::vec3 mPosition;
	/** Velocity of the spaceship. */
	glm::vec3 mVelocity;
	/** Time in seconds of last frame. */
	double    mTimeLastFrame;
};

#endif
