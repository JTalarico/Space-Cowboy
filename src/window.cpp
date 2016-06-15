/**
 * @file window.cpp
 *
 * Implementation file for the Window class.
 */
#include "window.hpp"

// Declare static data members.
Camera *Window::sPCamera = nullptr;

float Window::sLastCursorXPos = 0.0f;
float Window::sLastCursorYPos = 0.0f;

float   Window::lastFrame = 0.0f;
GLfloat Window::deltaTime = 0.0f;

float  Window::sMouseYaw        = -90.0f;
float  Window::sMousePitch      = 0.0f;

bool    Window::keysPressed[1024];
GLfloat Window::currentVelocity = 0.0f;
bool    Window::firstMouse      = true;
glm::vec3 Window::oldCameraFront = { 0.0f, 0.0f, 0.0f };

bool Window::collision = false;

glm::vec3 Window::bounce = glm::vec3(0.0f);

// Constructors.
Window::Window() :
		mWindow(nullptr) { }

Window::Window(int width, int height, const std::string& title) {
	// Create window. Throw exception if window could not be created.
	mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (not mWindow) {
		throw std::runtime_error("Window could not be created.");
	}

	// Define the viewport dimensions.
	int width_pixels, height_pixels;
	glfwGetFramebufferSize(mWindow, &width_pixels, &height_pixels);
	glViewport(0, 0, static_cast<GLsizei>(width_pixels), static_cast<GLsizei>(height_pixels));

	// Don't show cursor + make sure cursor cant leave window.
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set callback functions.
	glfwSetCursorPosCallback(mWindow, cursorCallback);
	glfwSetKeyCallback(mWindow, keyCallback);
	glfwSetScrollCallback(mWindow, scrollCallback);
	glfwSetMouseButtonCallback(mWindow, mouse_button_callback);
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}

// Destructors.
Window::~Window() {
	glfwDestroyWindow(mWindow);
}

// Accessor functions.
float Window::aspectRatio() const {
	return static_cast<float>(width()) / static_cast<float>(height());
}

bool Window::shouldClose() const {
	return glfwWindowShouldClose(mWindow) != 0;
}

int Window::width() const {
	int width, height;
	glfwGetFramebufferSize(mWindow, &width, &height);
	return width;
}

int Window::height() const {
	int width, height;
	glfwGetFramebufferSize(mWindow, &width, &height);
	return height;
}

// Mutator functions.
void Window::setCamera(Camera *pCamera) {
	sPCamera = pCamera;

	// Now that we have a camera:
	oldCameraFront = pCamera->direction();
}

// OpenGL modifier functions.
void Window::clear() const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::enable() const {
	glfwMakeContextCurrent(mWindow);
}

void Window::enableDepthTesting() const {
	glEnable(GL_DEPTH_TEST);
}

void Window::disable() const {
	glfwMakeContextCurrent(nullptr);
}

void Window::pollEvents() const {
	glfwPollEvents();
}

void Window::setBgColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a) const {
	glClearColor(r, g, b, a);
}

void Window::setBgColour(const std::array<GLfloat, 4>& bgColour) const {
	glClearColor(bgColour[0], bgColour[1], bgColour[2], bgColour[3]);
}

void Window::setBgColour(GLfloat *bgColour) const {
	glClearColor(bgColour[0], bgColour[1], bgColour[2], bgColour[3]);
}

void Window::setBgColour(const std::array<GLfloat, 3>& rgb, GLfloat a) {
	glClearColor(rgb[0], rgb[1], rgb[2], a);
}

void Window::setBgColour(GLfloat *rgb, GLfloat a) {
	glClearColor(rgb[0], rgb[1], rgb[2], a);
}

void Window::setViewport() const {
	glViewport(0, 0, static_cast<GLsizei>(width()), static_cast<GLsizei>(height()));
}

void Window::swapBuffers() const {
	glfwSwapBuffers(mWindow);
}

// Helper functions.
void Window::cursorCallback(GLFWwindow *, double xpos, double ypos) {
	if (firstMouse) {
		//	Initialize cursor positions
		sLastCursorXPos = static_cast<float>(xpos);
		sLastCursorYPos = static_cast<float>(ypos);
		firstMouse      = false;
	}

	GLfloat xoffset = static_cast<float>(xpos) - sLastCursorXPos;
	// Opposite order because y goes from bottom to left
	GLfloat yoffset = sLastCursorYPos - static_cast<float>(ypos);
	// Update last cursor position
	sLastCursorXPos = static_cast<float>(xpos);
	sLastCursorYPos = static_cast<float>(ypos);

	xoffset *= MOVE_SENSITIVITY;
	yoffset *= MOVE_SENSITIVITY;

	bool lTilt = sPCamera->getTiltLeft();
	bool rTilt = sPCamera->getTiltRight();
	
	if((lTilt && xoffset < 0) || (rTilt && xoffset > 0))
		sMouseYaw += xoffset;
	else if((lTilt && xoffset > 0) || (rTilt && xoffset < 0))
		sMouseYaw += xoffset / 6.0f;
	else 
		sMouseYaw += xoffset/3.0f;

		
	sMousePitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (sMousePitch > 89.9f) {
		sMousePitch = 89.9f;
	}
	if (sMousePitch < -89.9f) {
		sMousePitch = -89.9f;
	}

	glm::vec3 newDirection;
	newDirection.x = static_cast<float>(glm::cos(glm::radians(sMouseYaw)) *
	                                    glm::cos(glm::radians(sMousePitch)));
	newDirection.y = static_cast<float>(glm::sin(glm::radians(sMousePitch)));
	newDirection.z = static_cast<float>(glm::sin(glm::radians(sMouseYaw)) *
	                                    glm::cos(glm::radians(sMousePitch)));

	sPCamera->setDirection(glm::normalize(newDirection));
	if (!keysPressed[GLFW_KEY_LEFT_CONTROL]) {
		oldCameraFront = newDirection;
	}
}


void Window::keyCallback(GLFWwindow *window, int key, int, int action, int) {
	if (action == GLFW_PRESS) {
		keysPressed[key] = true;
	}
	if (action == GLFW_RELEASE) {
		keysPressed[key] = false;
	}


	// If ESC is pressed, close window.
	if (keysPressed[GLFW_KEY_ESCAPE]) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// If p is pressed, objects will be rendered with points. If l is pressed, objects will be
	// rendered with lines. If t is pressed, objects will be rendered with triangles.
	if (keysPressed[GLFW_KEY_P]) {
		// Increase point size to make points visible.
		glPointSize(POINT_SIZE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}

	if (keysPressed[GLFW_KEY_L]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (keysPressed[GLFW_KEY_T]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	moveCamera();
}

void Window::scrollCallback(GLFWwindow *, double, double) {
	// Nothing for now. May add a FOV changer
}

void Window::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			sPCamera->setTiltLeft(true);
		}
		else if (action == GLFW_RELEASE) {
			sPCamera->setTiltLeft(false);
		}
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			sPCamera->setTiltRight(true);
		}
		else if (action == GLFW_RELEASE) {
			sPCamera->setTiltRight(false);
		}
	}

}

void Window::updateDeltaTime() {
	GLfloat currentFrame = static_cast<float> (glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void Window::updatePosition() {
	glm::vec3 newPosition;

	glm::vec3 camPos = sPCamera->position();
	glm::vec3 camDir = sPCamera->direction();

	if (collision) {
		newPosition = sPCamera->position() + bounce;
		bounce      = glm::vec3(0.0f);
		currentVelocity /= 1.1f;
	}
	else {
		if (!keysPressed[GLFW_KEY_LEFT_CONTROL]) {
			newPosition =
					sPCamera->position() +
					sPCamera->direction() * (currentVelocity * deltaTime);
		}
		else {
			newPosition =
					sPCamera->position() +
					oldCameraFront * (currentVelocity * deltaTime);
		}
	}
	sPCamera->setPosition(newPosition);

}

void Window::setCollisison(bool collisionBool) {

	collision = collisionBool;
}

void Window::setBounce(glm::vec3 bounceEffect) {
	bounce = bounceEffect;
}

void Window::moveCamera() {
	// Inccrease/decrease camera move speed
	if (keysPressed[GLFW_KEY_W]) {
		if (currentVelocity < VELOCITY_CAP) {
			currentVelocity += VELOCITY_STEP;
		}
	}

	if (keysPressed[GLFW_KEY_S]) {
		if (currentVelocity > -VELOCITY_CAP) {
			currentVelocity -= VELOCITY_STEP;
		}
	}

	// Brake
	if (keysPressed[GLFW_KEY_SPACE]) {
		currentVelocity = 0;
	}



	// Added strafing for less awkward movement
	if (keysPressed[GLFW_KEY_A]) {
		glm::vec3 newPosition =
				          sPCamera->position() - glm::normalize(glm::cross(sPCamera->direction(),
				                                                           sPCamera->up())) *
				                                 (STRAFE_STEP * deltaTime);
		sPCamera->setPosition(newPosition);
	}

	if (keysPressed[GLFW_KEY_D]) {
		glm::vec3 newPosition =
				          sPCamera->position() + glm::normalize(glm::cross(sPCamera->direction(),
				                                                           sPCamera->up())) *
				                                 (STRAFE_STEP * deltaTime);
		sPCamera->setPosition(newPosition);
	}
}

