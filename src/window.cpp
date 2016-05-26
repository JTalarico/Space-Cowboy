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
	oldCameraFront = pCamera->cameraFront();
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
void Window::cursorCallback(GLFWwindow *window, double xpos, double ypos) {
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

	sMouseYaw += xoffset;
	sMousePitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (sMousePitch > 89.9f) {
		sMousePitch = 89.9f;
	}
	if (sMousePitch < -89.9f) {
		sMousePitch = -89.9f;
	}

	glm::vec3 newCameraFront;
	newCameraFront.x = static_cast<float> (cos(glm::radians(sMouseYaw)) * cos(glm::radians(sMousePitch)));
	newCameraFront.y = static_cast<float> (sin(glm::radians(sMousePitch)));
	newCameraFront.z = static_cast<float> (sin(glm::radians(sMouseYaw)) * cos(glm::radians(sMousePitch)));

	sPCamera->setCameraFront(glm::normalize(newCameraFront));
	if (!keysPressed[GLFW_KEY_LEFT_CONTROL]) {
		oldCameraFront = newCameraFront;
	}
}

void Window::keyCallback(GLFWwindow *window, int key, int, int action, int mods) {
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

void Window::scrollCallback(GLFWwindow *, double, double yoffset) {
	// Nothing for now. May add a FOV changer
}

void Window::updateDeltaTime() {
	GLfloat currentFrame = static_cast<float> (glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void Window::updatePosition() {
	glm::vec3 newPosition;
	if (!keysPressed[GLFW_KEY_LEFT_CONTROL]) {
		newPosition =
				sPCamera->position() +
				sPCamera->cameraFront() * (currentVelocity * deltaTime);
	}
	else {
		newPosition =
				sPCamera->position() +
				oldCameraFront * (currentVelocity * deltaTime);
	}
	sPCamera->setPosition(newPosition);
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
				          sPCamera->position() - glm::normalize(glm::cross(sPCamera->cameraFront(),
				                                                           sPCamera->up())) *
				                                 (STRAFE_STEP * deltaTime);
		sPCamera->setPosition(newPosition);
	}

	if (keysPressed[GLFW_KEY_D]) {
		glm::vec3 newPosition =
				          sPCamera->position() + glm::normalize(glm::cross(sPCamera->cameraFront(),
				                                                           sPCamera->up())) *
				                                 (STRAFE_STEP * deltaTime);
		sPCamera->setPosition(newPosition);
	}
}