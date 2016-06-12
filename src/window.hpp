/**
* @file window.hpp
*
* Interface file for the Window class.
*/
#ifndef SPACE_COWBOY_WINDOW_HPP
#define SPACE_COWBOY_WINDOW_HPP

#include "camera.hpp"

#include <array>
#include <stdexcept>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
* Represents an OpenGL window. A window should only be constructed after GLFW has been initialized.
* Note that the camera the window is to control should be set with the function setCamera.
*/
class Window {
public:
	// Constructors.
	/**
	* Creates a null window.
	*/
	Window();

	/**
	* Creates a window with the specified properties.
	*
	* @param width Width of the window in screen coordinates.
	* @param height Height of the window in screen coordinates.
	* @param title Title of the window.
	* @throws std::runtime_error if window could not be created.
	*/
	Window(int width, int height, const std::string& title);

	/**
	* Disallow copy constructor. GLFW only support deep copying of window objects, which is
	* dangerous since the Window destructor destroys a window's context.
	*/
	Window(const Window&) = delete;

	/**
	* Disallow copy assignment operator. GLFW only support deep copying of window objects, which
	* is dangerous since the Window destructor destroys a window's context.
	*/
	void operator=(const Window&) = delete;

	// Destructors.
	/**
	* Destroys window.
	*/
	~Window();

	// Accessor functions.
	/**
	* Returns the aspect ratio of the window.
	*/
	float aspectRatio() const;

	/**
	* Returns true iff window received a signal to close.
	*/
	bool shouldClose() const;

	/**
	* Returns the width of the window in pixels.
	*/
	int width() const;

	/**
	* Returns the height of the window in pixels.
	*/
	int height() const;

	// Mutator functions.
	/**
	* Sets the camera that the window will control.
	*
	* @param pCamera Pointer to Camera object window will control.
	*/
	void setCamera(Camera *pCamera);

	// OpenGL modifier functions.
	/**
	* Clears the screen, replacing the pixels with the window background colour.
	*/
	void clear() const;

	/**
	* Binds window to current OpenGL context.
	*/
	void enable() const;

	/**
	* Enables z-buffer depth testing. Must be called after window is enabled.
	*/
	void enableDepthTesting() const;

	/**
	* Binds null window to current OpenGL context, which unbinds any previously bound window
	* objects.
	*/
	void disable() const;

	/**
	* Polls for event triggers. If a event has been detected, the event's associated callback
	* function is called.
	*/
	void pollEvents() const;

	/**
	* Sets the window background colour in RGBA colour space.
	*
	* @param r Red value. Should be between 0.0 and 1.0.
	* @param g Green value. Should be between 0.0 and 1.0.
	* @param b Blue value. Should be between 0.0 and 1.0.
	* @param a Alpha value. Should be between 0.0 and 1.0.
	*/
	void setBgColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a) const;

	/**
	* Sets the window background colour in RGBA colour space.
	*
	* @param bgColour RGBA values.
	*/
	void setBgColour(const std::array<GLfloat, 4>& bgColour) const;

	/**
	* Sets the window background colour in RGBA colour space.
	*
	* @param bgColour RGBA values.
	*/
	void setBgColour(GLfloat *bgColour) const;

	/**
	* Sets the window background colour in RGBA colour space.
	*
	* @param rgb RGB values.
	* @param a Alpha value.
	*/
	void setBgColour(const std::array<GLfloat, 3>& rgb, GLfloat a);

	/**
	* Sets the window background colour in RGBA colour space.
	*
	* @param rgb RGB values.
	* @param a Alpha value.
	*/
	void setBgColour(GLfloat *rgb, GLfloat a);

	/**
	* Sets the viewport dimensions to match window dimensions. Should be called if window
	* dimensions change.
	*/
	void setViewport() const;

	/**
	* Swaps the front and back buffers.
	*/
	void swapBuffers() const;

	/**
	* Updates the time between the last and current frame (deltaTime).
	*/
	void updateDeltaTime();

	/**
	* Updates the position to the camera based on the current velocity.
	*/
	void updatePosition();

	void setCollisison(bool collision);

	void setBounce(glm::vec3 bounceEffect);

private:
	// Data members.
	/** Pointer to a GLFW window object. */
	GLFWwindow *mWindow;

	/** Pointer to the camera object that window will control. */
	static Camera *sPCamera;

	/** x coordinate of last cursor. */
	static float sLastCursorXPos;
	/** y coordinate of last cursor. */
	static float sLastCursorYPos;

	/** Array that keeps track of which keys are pressed*/
	static bool      keysPressed[1024];  // No need to initialize since static bool arrays are
										 // false by default.
										 /** Current mouse yaw */
	static float     sMouseYaw;
	/** Current mouse pitch */
	static float     sMousePitch;
	/** True if this is the first tie using mouse */
	static bool      firstMouse;
	/** Camera front used when ctrl is held down. */
	static glm::vec3 oldCameraFront;
	/** Time of last frame */
	static float     lastFrame;
	/** Time difference between last and current frame */
	static GLfloat   deltaTime;
	/** Current velocity of camera (space ship). */
	static GLfloat   currentVelocity;

	static bool collision;

	static glm::vec3 bounce;

	// Callback constants.
	/** Size of points when rendering points. */
	static constexpr float POINT_SIZE = 3.0f;
	/** How far the camera moves when zooming with the cursor. */
	static constexpr float ZOOM_SENSITIVITY = 0.02f;
	/** How far camera moves when scroll wheel is used. */
	static constexpr float SCROLL_SENSITIVITY = 0.5f;
	/** How much velocity increases/decreases at each button press */
	static constexpr float VELOCITY_STEP = 1.0f;
	/** How much velocity increases/decreases at each button press */
	static constexpr float VELOCITY_CAP = 2000.0f;
	/** How much camera strafes with each button press */
	static constexpr float STRAFE_STEP = 0.5f;
	/** How fast camera moves when moving with the mouse */
	static constexpr float MOVE_SENSITIVITY = 0.1f;

	// Helper functions.
	/**
	* Called at the end of key_callback.
	*
	* Decides how the postion of the camera is affected based on which key is pressed.
	*
	*/
	static void moveCamera();

	/**
	* Called when the cursor moves.
	*
	* If the left button is pressed, the camera move in the negative z direction if the cursor is
	* moving upwards, and the camera will move in the positive z direction if the cursor is moving
	* downwards.
	*
	* @param xpos x position of cursor on screen.
	* @param ypos y position of cursor on screen.
	*/
	static void cursorCallback(GLFWwindow *, double xpos, double ypos);

	/**
	* Called when a key is pressed.
	*
	* If p is pressed, objects will be rendered with points. If w is pressed, objects will be
	* rendered with lines. If t is pressed, objects will be rendered with triangles.
	*
	* @param window Pointer to GLFW window.
	* @param key Key that was pressed.
	* @param action Action on key that was performed.
	*/
	static void keyCallback(GLFWwindow *window, int key, int, int action, int);

	/**
	* Called when user scrolls with either a mouse wheel or touchpad gesture.
	*
	* Scrolling up moves the camera along the negative z axis, while scrolling down moves the
	* camera along the positive z axis.
	*
	*/
	static void scrollCallback(GLFWwindow *, double, double);

};

#endif

