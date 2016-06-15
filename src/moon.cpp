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

void Moon::setMoonTexture() {
	std::random_device rd;
	std::mt19937       e2(rd());

	// Generate random noise
	std::normal_distribution<float> randomLight(0.7f, 0.2f);
	std::normal_distribution<float> randomDark(0.3f, 0.2f);

	glm::vec3 primaryColor {randomLight(e2)};
	glm::vec3 secondaryColor {randomDark(e2)};

	double xPeriod   = 0;
	double yPeriod   = 0;

	double turbPower = 100.0;
	double turbSize  = 32.0;

	generateTexture(primaryColor, secondaryColor, xPeriod, yPeriod, turbPower, turbSize);
	updateBuffers();
}

bool Moon::moonCollision(const Camera& camera) {
	
	glm::vec3 cameraPosition = camera.position();
	glm::vec3 camDir = camera.direction();
	glm::vec3 moonCenter = position();
	float     moonRadius = glm::length(mScale * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

	if (glm::length(glm::vec3(cameraPosition.x, cameraPosition.y - 4.0f, cameraPosition.z)
		+ glm::vec3(25 * camDir.x, 25 * camDir.y, 25 * camDir.z) - moonCenter) <=
		moonRadius * 1.1f) {
		return true;
	}

	return false;
}
