/**
 * @file program.hpp
 *
 * Interface file for the Program class.
 */
#ifndef SPACE_COWBOY_PROGRAM_HPP
#define SPACE_COWBOY_PROGRAM_HPP

#include "utility.hpp"

#include <stdexcept>
#include <string>
#include <GL/glew.h>

/**
 * Represents a shader program. A program should only be constructed after OpenGL has initialized.
 */
class Program {
public:
	// Constructors.
	/**
	 * Creates a null program.
	 */
	Program();

	/**
	 * Creates a shader program from a vertex shader and fragment shader whose source codes are
	 * contained in the provided file paths.
	 *
	 * @param vertexShaderPath Path to vertex shader source code.
	 * @param fragmentShaderPath Path to fragment shader source code.
	 * @throws std::runtime_error if Program cannot be constructed.
	 */
	Program(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

	/**
	 * Disallow copy constructor. OpenGL does not provide any mechanism for shallow copying of
	 * shader programs.
	 */
	Program(const Program&) = delete;

	/**
	 * Disallow copy assignment operator. OpenGL does not provide any mechanism for shallow copying
	 * of shader programs.
	 */
	void operator=(const Program&) = delete;

	// Destructor.
	/**
	 * Destroys the shader program.
	 */
	~Program();

	// Accessor functions.
	/**
	 * Returns the location of the uniform with the provided name.
	 *
	 * @param uniformName Name of uniform.
	 * @return Location of uniform in shader program.
	 * @throws std::runtime_error if specified uniform does not exits.
	 */
	GLint getUniformLocation(const std::string& uniformName) const;

	// OpenGL modifiers.
	/**
	 * Binds shader program to current OpenGL context.
	 */
	void enable() const;

	/**
	 * Binds null shader program to current OpenGL context, which unbinds any previously bound
	 * programs.
	 */
	void disable() const;

private:
	// Data members.
	/** Reference ID for the program. */
	GLuint mProgramID;

	// Static helper functions.
	/**
	 * Returns true iff shader object's source code compiled successfully.
	 *
	 * @param shaderID Reference ID of shader object.
	 * @return True iff shader object's source code compiled correctly.
	 */
	static bool compilationSuccessful(GLuint shaderID);
};

#endif
