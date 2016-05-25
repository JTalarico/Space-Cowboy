/**
 * @file window.cpp
 *
 * Implementation file for the Window class.
 */
#include "window.hpp"

// Declare static data members.
Camera *Window::sPCamera   = nullptr;
double Window::sCursorXPos = 0.0;
double Window::sCursorYPos = 0.0;

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
	// If the left mouse button is pressed, subtract the current cursor height from the cursor
	// height in the last frame. Use this difference to adjust the camera position.
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		float deltaY = static_cast<float>(ypos - sCursorYPos);

		glm::vec3 newPosition =
				          sPCamera->position() + (ZOOM_SENSITIVITY) * deltaY * glm::vec3(0, 0, 1);
		sPCamera->setPosition(newPosition);
	}

	// Update the cursor position.
	sCursorXPos = xpos;
	sCursorYPos = ypos;
}

void Window::keyCallback(GLFWwindow *window, int key, int, int action, int) {
	// If ESC is pressed, close window.
	if (key == GLFW_KEY_ESCAPE and action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// If p is pressed, objects will be rendered with points. If w is pressed, objects will be
	// rendered with lines. If t is pressed, objects will be rendered with triangles.
	if (key == GLFW_KEY_P and action == GLFW_PRESS) {
		// Increase point size to make points visible.
		glPointSize(POINT_SIZE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}

	if (key == GLFW_KEY_W and action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (key == GLFW_KEY_T and action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Window::scrollCallback(GLFWwindow *, double, double yoffset) {
	glm::vec3 newCameraPosition =
			          sPCamera->position() +
			          glm::vec3(0, 0, SCROLL_SENSITIVITY * static_cast<float>(yoffset));
	sPCamera->setPosition(newCameraPosition);
}
