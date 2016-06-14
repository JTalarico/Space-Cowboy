/**
 * @file camera.hpp
 *
 * Interface file for the Camera class.
 */
#ifndef SPACE_COWBOY_CAMERA_HPP
#define SPACE_COWBOY_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * Class for accessing and manipulating the state of the camera. A camera object can create a view
 * and projection matrix when constructed with the necessary parameters, such as field of view and
 * camera position.
 */
class Camera {
public:
	// Constructors.
	/**
	 * Creates a camera situated at the origin and oriented along the negative z direction, with a
	 * field of view of 45 degrees and an aspect ratio of 1.
	 */
	Camera();

	/**
	 * Creates a camera with the specified parameters.
	 *
	 * @param fov Field of view in radians.
	 * @param aspectRatio Aspect ratio.
	 * @param nearClippingPlane Distance to the front side of the viewing frustum.
	 * @param farClippingPlane Distance to the back side of the viewing frustum.
	 * @param position Camera's position.
	 * @param direction Direction camera is facing at.
	 * @param up Upwards direction of camera.
	 */
	Camera(float fov, float aspectRatio, float nearClippingPlane, float farClippingPlane,
	       const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up);

	// Accessor functions.
	/** Returns the camera's position.
	 *
	 * @return Position of camera.
	 */
	glm::vec3 position() const;

	/**
	 * Returns the direction the camera is facing.
	 *
	 * @return Direction camera is facing.
	 */
	glm::vec3 direction() const;

	/**
	 * Returns the camera's up vector.
     *
     * @return Up vector of camera.
     */
	glm::vec3 up() const;

	/**
	 * Returns a view matrix defined by the camera's position and orientation.
	 *
	 * @return View matrix.
	 */
	glm::mat4 view() const;

	/**
	 * Returns a projection matrix using perspective projection.
	 *
	 * @returns Projection matrix.
	 */
	glm::mat4 projection() const;

	// Mutator functions.
	/**
	 * Sets the camera's field of view.
	 *
	 * @param fov Camera's new field of view.
	 */
	void setFOV(float fov);

	/**
	 * Sets the camera's aspect ratio.
	 *
	 * @param aspectRatio Camera's new aspect ratio.
	 */
	void setAspectRatio(float aspectRatio);

	/**
	 * Sets the camera's near clipping plane.
	 *
	 * @param nearClippingPlane Camera's new near clipping plane.
	 */
	void setNearClippingPlane(float nearClippingPlane);

	/**
	 * Sets the camera's far clipping plane.
	 *
	 * @param farClippingPlane Camera's new far clipping plane.
	 */
	void setFarClippingPlane(float farClippingPlane);

	/**
	 * Sets the camera's position.
	 *
	 * @param position Camera's new position.
	 */
	void setPosition(const glm::vec3& position);

	/**
	 * Sets the direction the camera is facing.
	 *
	 * @param direction Camera's new look at point.
	 */
	void setDirection(const glm::vec3& direction);

	/**
	 * Sets the direction perpendicular to the camera's look at point and position.
	 *
	 * @param up Camera's new up direction.
	 */
	void setUp(const glm::vec3& up);

	bool getTiltLeft() const;

	bool getTiltRight() const;

	void setTiltLeft(bool tilt) ;

	void setTiltRight(bool tilt) ;

private:
	/** Camera field of view. */
	float mFOV;
	/** Window aspect ratio. */
	float mAspectRatio;
	/** Near clipping plane is the distance to the front side of the viewing frustum. */
	float mNearClippingPlane;
	/** Far clipping plane is the distance to the back side of the viewing frustum. */
	float mFarClippingPlane;

	bool shipTiltLeft;
	bool shipTiltRight;
	/** Camera position. */
	glm::vec3 mPosition;
	/** Direction camera is facing. */
	glm::vec3 mDirection;
	/** Points in a direction perpendicular to (mPosition + mDirection) and mPosition. */
	glm::vec3 mUp;
};

#endif
