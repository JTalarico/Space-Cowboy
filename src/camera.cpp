/**
 * @file camera.cpp
 *
 * Implementation file for the Camera class.
 */
#include "camera.hpp"

// Constructors.
Camera::Camera() :
		mFOV(glm::radians(45.0f)),
		mAspectRatio(1.0f),
		mNearClippingPlane(0.1f),
		mFarClippingPlane(100.0f),
		mPosition(glm::vec3(0, 0, 0)),
		mDirection(glm::vec3(0, 0, -1.0f)),
		mUp(glm::vec3(0, 1, 0)) { }

Camera::Camera(float fov, float aspectRatio, float nearClippingPlane, float farClippingPlane,
               const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up) :
		mFOV(fov),
		mAspectRatio(aspectRatio),
		mNearClippingPlane(nearClippingPlane),
		mFarClippingPlane(farClippingPlane),
		mPosition(position),
		mDirection(direction),
		mUp(up) { }

// Accessor functions.
glm::vec3 Camera::position() const {
	return mPosition;
}

glm::vec3 Camera::direction() const {
	return mDirection;
}

glm::vec3 Camera::up() const {
	return mUp;
}

glm::mat4 Camera::view() const {
	return glm::lookAt(mPosition, mPosition + mDirection, mUp);
}

glm::mat4 Camera::projection() const {
	return glm::perspective(mFOV, mAspectRatio, mNearClippingPlane, mFarClippingPlane);
}

// Mutator functions.
void Camera::setFOV(float fov) {
	mFOV = fov;
}

void Camera::setAspectRatio(float aspectRatio) {
	mAspectRatio = aspectRatio;
}

void Camera::setNearClippingPlane(float nearClippingPlane) {
	mNearClippingPlane = nearClippingPlane;
}

void Camera::setFarClippingPlane(float farClippingPlane) {
	mFarClippingPlane = farClippingPlane;
}

void Camera::setPosition(const glm::vec3& position) {
	mPosition = position;
}

void Camera::setDirection(const glm::vec3& direction) {
	mDirection = direction;
}

void Camera::setUp(const glm::vec3& up) {
	mUp = up;
}
