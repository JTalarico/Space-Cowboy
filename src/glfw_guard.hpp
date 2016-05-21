/**
 * @file glfw_guard.hpp
 *
 * Interface file for the GLFWGuard class.
 */
#ifndef SPACE_COWBOY_GLFW_GUARD_HPP
#define SPACE_COWBOY_GLFW_GUARD_HPP

#include <stdexcept>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 * RAII handler for the GLFW library. Constructor initializes the library, while the destructor
 * terminates the library. Multiple attempts to initialize the GLFW library are prevented.
 */
class GLFWGuard {
public:
	// Constructors.
	/**
	 * Initializes the GLFW library, unless it has already been initialized.
	 *
	 * @throws std::runtime_error if GLFW cannot be initialized.
	 */
	GLFWGuard();

	/**
	 * Disallow copy construction, as copying an RAII guard does not make much sense.
	 */
	GLFWGuard(const GLFWGuard&) = delete;

	/**
	 * Disallow copy assignment operator, as copying an RAII guard does not make much sense.
	 */
	void operator=(const GLFWGuard&) = delete;

	// Destructors.
	/**
	 * Terminates the GLFW library.
	 */
	~GLFWGuard();

private:
	// Static data members.
	/** True iff GLFW has been initialized. */
	static bool sGLFWInitialized;
};

#endif
