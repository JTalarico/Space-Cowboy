/**
 * @file spaceship.cpp
 *
 * Implementation file for the Spaceship class.
 */
#include "spaceship.hpp"

// Constructors.
Spaceship::Spaceship() :
		mPosition(),
		mVelocity(),
		mTimeLastFrame(glfwGetTime()) { }

// Accessor functions.
glm::vec3 Spaceship::position() const {
	return mPosition;
}

glm::vec3 Spaceship::velocity() const {
	return mVelocity;
}

// Mutator functions.
void Spaceship::setVelocity(const glm::vec3& velocity) {
	mVelocity = velocity;
}

void Spaceship::translate(const glm::vec3& displacement) {
	mPosition += displacement;
}

void Spaceship::updateState() {
	double currentTime = glfwGetTime();
	float  deltaT      = static_cast<float>(currentTime - mTimeLastFrame);

	mPosition += deltaT * mVelocity;

	mTimeLastFrame = currentTime;
}
