/**
 * @file sun.cpp
 *
 * Implementation file for the Sun class.
 */
#include "sun.hpp"

namespace {
// Shader program file paths.
/** Path to vertex shader source code. */
constexpr char VERTEX_SHADER_PATH[]   = "shaders/sun_vertex.shader";
/** Path to fragment shader source code. */
constexpr char FRAGMENT_SHADER_PATH[] = "shaders/sun_fragment.shader";

// Sphere properties.
/** Number of lines of latitude. */
constexpr unsigned int N_LATITUDE  = 120;
/** Number of lines of longitude. */
constexpr unsigned int N_LONGITUDE = 240;
}

// Constructors.
Sun::Sun() :
		mProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH),
		mScale() {
	// Create the Sphere object which holds the sun's vertex, normal, and index data. Record the
	// number of vertex components and indices.
	Sphere sphere(1.0f, N_LATITUDE, N_LONGITUDE);
	mNVertices = static_cast<unsigned int>(sphere.vertices.size());
	mNIndices  = static_cast<unsigned int>(sphere.indices.size());

	// Create vertex array buffer, vertex buffer object, and element buffer objects and bind
	// them to current OpenGL context.
	glGenVertexArrays(1, &sVAO);
	glGenBuffers(1, &sVBO);
	glGenBuffers(1, &sEBO);
	glBindVertexArray(sVAO);
	glBindBuffer(GL_ARRAY_BUFFER, sVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sEBO);

	// Pass vertex data into vertex buffer object.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mNVertices,
	             static_cast<GLvoid *>(sphere.vertices.data()), GL_STATIC_DRAW);

	// Pass index data into element buffer object.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mNIndices,
	             static_cast<GLvoid *>(sphere.indices.data()), GL_STATIC_DRAW);

	// Create and enable vertex attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
	                      static_cast<GLvoid *>(0));
	glEnableVertexAttribArray(0);

	// Unbind vertex array buffer, vertex buffer object, and element buffer objects.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Set colour and opacity.
	setColour(palette::YELLOW);
	setOpacity(palette::OPAQUE);

}

// Destructors.
Sun::~Sun() {
	glDeleteVertexArrays(1, &sVAO);
	glDeleteBuffers(1, &sVBO);
	glDeleteBuffers(1, &sEBO);
}

// Accessor functions.
glm::mat4 Sun::modelMatrix() const {
	return mScale;
}

// Modifier functions.
void Sun::scale(float scaleFactor) {
	mScale = glm::scale(mScale, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
}

void Sun::setColour(GLfloat r, GLfloat g, GLfloat b) const {
	// Get "objectColour" uniform location, enable program, set uniform value, and disable program.
	GLint objectColourUniformLocation = mProgram.getUniformLocation("objectColour");
	mProgram.enable();
	glUniform3f(objectColourUniformLocation, r, g, b);
	mProgram.disable();
}

void Sun::setColour(const std::array<GLfloat, 3>& colour) const {
	setColour(colour[0], colour[1], colour[2]);
}

void Sun::setOpacity(GLfloat alpha) const {
	// Get "objectOpacity" uniform location, enable program, set uniform value, and disable program.
	GLint objectOpacityUniformLocation = mProgram.getUniformLocation("objectOpacity");
	mProgram.enable();
	glUniform1f(objectOpacityUniformLocation, alpha);
	mProgram.disable();
}

// OpenGL modifiers.
void Sun::draw(const Camera& camera) const {
	// Enable program.
	mProgram.enable();

	// Calculate the model-view-projection matrix and set the corresponding uniform.
	glm::mat4 model      = modelMatrix();
	glm::mat4 view       = camera.view();
	glm::mat4 projection = camera.projection();

	glm::mat4 MVP = projection * view * model;

	glUniformMatrix4fv(mProgram.getUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

	// Bind vertex array object and element buffer object to current context.
	glBindVertexArray(sVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sEBO);

	// Draw.
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mNIndices), GL_UNSIGNED_INT,
	               static_cast<GLvoid *>(0));

	// Disable program and unbind vertex array object and element buffer object.
	mProgram.disable();
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
