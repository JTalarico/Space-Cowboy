/**
 * @file window.cpp
 *
 * Implementation file for the Window class.
 */
#include "window.hpp"
#include <iostream>

// Declare static data members.
Camera *Window::sPCamera = nullptr;

double Window::sLastCursorXPos = 0;
double Window::sLastCursorYPos = 0;

double  Window::lastFrame = 0.0;
GLfloat Window::deltaTime = 0.0f;

double  Window::sMouseYaw       = -90.0;
double  Window::sMousePitch     = 0.0;
GLfloat Window::currentVelocity = 0.0;
bool    Window::firstMouse      = true;
bool Window::ctrlDown = false;
glm::vec3 Window::oldCameraFront = {0,0,0};
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
		sLastCursorXPos = xpos;
		sLastCursorYPos = ypos;
		firstMouse      = false;
	}

	GLfloat xoffset = xpos - sLastCursorXPos;
	GLfloat yoffset = sLastCursorYPos - ypos; // Opposite order because y goes from bottom to left
	// Update last cursor position
	sLastCursorXPos = xpos;
	sLastCursorYPos = ypos;

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
	newCameraFront.x = cos(glm::radians(sMouseYaw)) * cos(glm::radians(sMousePitch));
	newCameraFront.y = sin(glm::radians(sMousePitch));
	newCameraFront.z = sin(glm::radians(sMouseYaw)) * cos(glm::radians(sMousePitch));

	sPCamera->setCameraFront(glm::normalize(newCameraFront));
	if (!ctrlDown)
		oldCameraFront = newCameraFront;

}

void Window::keyCallback(GLFWwindow *window, int key, int, int action, int mods) {
	// If ESC is pressed, close window.
	if (key == GLFW_KEY_ESCAPE and action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// If p is pressed, objects will be rendered with points. If l is pressed, objects will be
	// rendered with lines. If t is pressed, objects will be rendered with triangles.
	if (key == GLFW_KEY_P and action == GLFW_PRESS) {
		// Increase point size to make points visible.
		glPointSize(POINT_SIZE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}

	if (key == GLFW_KEY_L and action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (key == GLFW_KEY_T and action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// Inccrease/decrease camera move speed
	if (key == GLFW_KEY_W and (action == GLFW_PRESS or action == GLFW_REPEAT)) {
		if (currentVelocity < VELOCITY_CAP) {
			currentVelocity += VELOCITY_STEP;
		}
	}

	if (key == GLFW_KEY_S and (action == GLFW_PRESS or action == GLFW_REPEAT)) {
		if (currentVelocity > -VELOCITY_CAP) {
			currentVelocity -= VELOCITY_STEP;
		}
	}

	// Brake
	if (key == GLFW_KEY_SPACE and action == GLFW_PRESS) {
		currentVelocity = 0;
	}

	// Added strafing for less awkward movement
	if (key == GLFW_KEY_A and (action == GLFW_PRESS or action == GLFW_REPEAT)) {
		glm::vec3 newPosition =
				          sPCamera->position() - glm::normalize(glm::cross(sPCamera->cameraFront(),
				                                                           sPCamera->up())) *
				                                 (STRAFE_STEP * deltaTime);
		sPCamera->setPosition(newPosition);
	}

	if (key == GLFW_KEY_D and (action == GLFW_PRESS or action == GLFW_REPEAT)) {
		glm::vec3 newPosition =
				          sPCamera->position() + glm::normalize(glm::cross(sPCamera->cameraFront(),
				                                                           sPCamera->up())) *
				                                 (STRAFE_STEP * deltaTime);
		sPCamera->setPosition(newPosition);
	}
	if (key == GLFW_KEY_LEFT_CONTROL and action == GLFW_PRESS)
		ctrlDown = true;
	if (key == GLFW_KEY_LEFT_CONTROL and action == GLFW_RELEASE)
		ctrlDown = false;
}

void Window::scrollCallback(GLFWwindow *, double, double yoffset) {
	glm::vec3 newCameraPosition =
			          sPCamera->position() +
			          glm::vec3(0, 0, SCROLL_SENSITIVITY * static_cast<float>(yoffset));
	sPCamera->setPosition(newCameraPosition);
}

void Window::updateDeltaTime() {
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void Window::updatePosition() {
	glm::vec3 newPosition;
	if (!ctrlDown)
		newPosition =
			          sPCamera->position() +
			          sPCamera->cameraFront() * (currentVelocity * deltaTime);
	else
		newPosition =
				          sPCamera->position() +
				          oldCameraFront * (currentVelocity * deltaTime);
	sPCamera->setPosition(newPosition);
}