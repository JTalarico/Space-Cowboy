/**
 * @file glfw_guard.cpp
 *
 * Implementation file for the GLFWGuard class.
 */
#include "glfw_guard.hpp"

// Initialize static data members.
bool GLFWGuard::sGLFWInitialized = false;

// Constructors.
GLFWGuard::GLFWGuard() {
	if (not sGLFWInitialized) {
		if (not glfwInit()) {
			throw std::runtime_error("Failed to start GLFW.");
		}

		// Set required OpenGL version and request core profile.
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 8);
		glEnable(GL_MULTISAMPLE);
		//glEnable(GL_BLEND);
	}

	sGLFWInitialized = true;
}

// Destructors.
GLFWGuard::~GLFWGuard() {
	glfwTerminate();
	sGLFWInitialized = false;
}
