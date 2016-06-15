/**
 * @file moon.hpp
 *
 * Interface file for the Moon class.
 */
#ifndef SPACE_COWBOY_MOON_HPP
#define SPACE_COWBOY_MOON_HPP

#include "planet.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
/**
 * Represents a Moon.
 */
class Moon : public Planet {
public:
	// Mutator functions.
	/**
	 * Rotates the moon around a point.
	 *
	 * @param point Point to rotate around.
	 * @param angle Angle to rotate by in radians.
	 * @param rotationAxis Axis to rotate around.
	 */
	void rotateAroundPoint(const glm::vec3& point, float angle, const glm::vec3& rotationAxis);

	/**
	 * Set the planet the moon orbits around.
	 *
	 * @param pPlanet Pointer to planet moon orbits around.
	 */
	void setPrimary(Planet *pPlanet);

	/**
	 * Updates the moon's state.
	 */
	void updateState();

	/**
	 * Sets the moon's texture, and updates the buffers.
	 */
	void setMoonTexture();
private:
	// Data members.
	/** Pointer to planet the moon is orbiting. */
	Planet *mPrimary;
};

#endif
