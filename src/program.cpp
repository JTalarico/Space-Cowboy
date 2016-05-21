/**
 * @file program.cpp
 *
 * Implementation file for the Program class.
 */
#include "program.hpp"

// Constructors.
Program::Program() :
		mProgramID(0) { }

Program::Program(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
	// Read vertex shader source code and fragment shader source code.
	const std::string vertexShaderSourceCode   = utility::readFile(vertexShaderPath);
	const std::string fragmentShaderSourceCode = utility::readFile(fragmentShaderPath);

	// Create vertex shader and fragment shader objects.
	GLuint vertexShaderID   = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Attach shader source code to their respective shader objects and compile.
	// Note that source code must be converted into a C string so that the address of a C string can
	// be passed to glShaderSource.
	const char *vertexShaderSourceCode_cstr   = vertexShaderSourceCode.c_str();
	const char *fragmentShaderSourceCode_cstr = fragmentShaderSourceCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexShaderSourceCode_cstr, nullptr);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderSourceCode_cstr, nullptr);
	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	// Throw exception if compilation was unsuccessful.
	if (not compilationSuccessful(vertexShaderID)) {
		std::string exceptionMessage = "Vertex shader failed to compile.";
		throw std::runtime_error(exceptionMessage.c_str());
	}
	if (not compilationSuccessful(fragmentShaderID)) {
		std::string exceptionMessage = "Fragment shader failed to compile.";
		throw std::runtime_error(exceptionMessage);
	}

	// Link vertex shader and fragment shader objects.
	mProgramID = glCreateProgram();
	glAttachShader(mProgramID, vertexShaderID);
	glAttachShader(mProgramID, fragmentShaderID);
	glLinkProgram(mProgramID);

	// Delete shader objects.
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	// Throw exception if program did not link correctly.
	GLint success;
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &success);
	if (not success) {
		std::string exceptionMessage = "Shader program failed to link.";
		throw std::runtime_error(exceptionMessage);
	}
}

// Destructor.
Program::~Program() {
	glDeleteProgram(mProgramID);
}

// Accessor functions.
GLint Program::getUniformLocation(const std::string& uniformName) const {
	GLint uniformLocation = glGetUniformLocation(mProgramID, uniformName.c_str());
	if (uniformLocation == -1) {
		std::string errorMessage = "Uniform with name \"" + uniformName + "\" does not exist.";
		throw std::runtime_error(errorMessage);
	}
	return uniformLocation;
}

// OpenGL modifiers.
void Program::enable() const {
	glUseProgram(mProgramID);
}

void Program::disable() const {
	glUseProgram(0);
}

// Static helper functions.
bool Program::compilationSuccessful(GLuint shaderID) {
	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	return success != 0;
}
