/**
 * @file moon.cpp
 *
 * Implementation file for the Moon class.
 */
#include "moon.hpp"

void Moon::rotateAroundPoint(const glm::vec3& point, float angle, const glm::vec3& rotationAxis) {
	glm::mat4 orbitalRotation = glm::rotate(glm::mat4(), angle, rotationAxis);
	glm::vec3 oldPosition     = position() - point;

	// Translate moon back to point, rotate the old position vector, and translate back to the
	// new position.
	glm::vec3 newPosition = glm::vec3(orbitalRotation * glm::vec4(oldPosition, 1.0f));
	mTranslation = glm::translate(mTranslation, -oldPosition);
	mTranslation = glm::translate(mTranslation, newPosition);
}

void Moon::setPrimary(Planet *pPlanet) {
	mPrimary = pPlanet;
}

void Moon::updateState() {
	double currentTime = glfwGetTime();
	float  deltaT      = static_cast<float>(currentTime - mTimeLastStateUpdate);

	// Move the moon so that it is the same relative position from the primary planet as in the
	// previous frame.
	if (glm::length(mPrimary->orbitalAngularVelocity()) > 0.0f) {
		float rotationalAngle = glm::length(mPrimary->orbitalAngularVelocity()) * deltaT;
		rotateAroundOrigin(rotationalAngle, mPrimary->orbitalAngularVelocity());
	}

	// Rotate moon about its own centre according to the moon's angular velocity and time since
	// last state update.
	if (glm::length(mAngularVelocity) > 0.0f) {
		float rotationalAngle = glm::length(mAngularVelocity) * deltaT;
		rotate(rotationalAngle, mAngularVelocity);
	}

	// Rotate moon about the planet according to the moon's orbital angular velocity and the time
	// since the last state update.
	if (glm::length(mOrbitalAngularVelocity) > 0.0f) {
		float orbitalAngle = glm::length(mOrbitalAngularVelocity) * deltaT;
		rotateAroundPoint(mPrimary->position(), orbitalAngle, mOrbitalAngularVelocity);
	}

	mTimeLastStateUpdate = currentTime;
}
