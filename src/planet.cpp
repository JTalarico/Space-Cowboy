/**
 * @file planet.cpp
 *
 * Implementation file for the Planet class.
 */
#include "planet.hpp"

namespace {
// Shader program file paths.
/** Path to vertex shader source code. */
constexpr char VERTEX_SHADER_PATH[]   = "shaders/vertex.shader";
/** Path to fragment shader source code. */
constexpr char FRAGMENT_SHADER_PATH[] = "shaders/fragment.shader";

// Sphere properties.
/** Number of lines of latitude. */
constexpr unsigned int NLATITUDE  = 120;
/** Number of lines of longitude. */
constexpr unsigned int NLONGITUDE = 240;
}

// Constructors.
Planet::Planet() :
		mProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH),
		mScale(),
		mRotation(),
		mTranslation(),
		mAngularVelocity(),
		mOrbitalAngularVelocity(),
		mTimeLastStateUpdate(glfwGetTime()) {
	// Load the vertex and index data from Sphere object.
	Sphere sphere(1.0f, NLATITUDE, NLONGITUDE);
	mVertices = sphere.vertices;
	mIndices  = sphere.indices;

	// Create vertex array buffer, vertex buffer object, and element buffer objects and bind them to
	// current OpenGL context.
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);

	// Pass vertex data into vertex buffer object.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mVertices.size(),
	             static_cast<GLvoid *>(mVertices.data()), GL_STATIC_DRAW);

	// Pass index data into element buffer object.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mIndices.size(),
	             static_cast<GLvoid *>(mIndices.data()), GL_STATIC_DRAW);

	// Create and enable vertex attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid *>(0));
	glEnableVertexAttribArray(0);

	// Unbind vertex array buffer, vertex buffer object, and element buffer objects.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Set colour and opacity.
	setColour(palette::BLUE);
	setOpacity(palette::OPAQUE);
}

Planet::Planet(const Planet& other) :
		mProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) {
	mScale       = other.mScale;
	mRotation    = other.mRotation;
	mTranslation = other.mTranslation;

	mAngularVelocity        = other.mAngularVelocity;
	mOrbitalAngularVelocity = other.mOrbitalAngularVelocity;
}

Planet& Planet::operator=(const Planet& other) {
	mScale       = other.mScale;
	mRotation    = other.mRotation;
	mTranslation = other.mTranslation;

	mAngularVelocity        = other.mAngularVelocity;
	mOrbitalAngularVelocity = other.mOrbitalAngularVelocity;

	return *this;
}

// Destructors.
Planet::~Planet() {
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mEBO);
}

// Accessor functions.
glm::vec3 Planet::angularVelocity() const {
	return mAngularVelocity;
}

glm::mat4 Planet::modelMatrix() const {
	return mRotation * mTranslation * mScale;
}

glm::vec3 Planet::orbitalAngularVelocity() const {
	return mOrbitalAngularVelocity;
}

glm::vec3 Planet::position() const {
	return glm::vec3(mTranslation[3]);
}

// Planet modifier functions.
void Planet::rotate(float angle, const glm::vec3& rotationAxis) {
	mRotation = glm::rotate(mRotation, angle, rotationAxis);
}

void Planet::rotate(float angle, float nx, float ny, float nz) {
	rotate(angle, glm::vec3(nx, ny, nz));
}

void Planet::rotateAroundOrigin(float angle, const glm::vec3& rotationAxis) {
	glm::mat4 orbitalRotation = glm::rotate(glm::mat4(), angle, rotationAxis);
	glm::vec3 oldPosition     = position();

	// Translate planet back to origin, rotate the old position vector, and translate back to the
	// new position.
	glm::vec3 newPosition = glm::vec3(orbitalRotation * glm::vec4(oldPosition, 1.0f));
	mTranslation = glm::translate(mTranslation, -oldPosition);
	mTranslation = glm::translate(mTranslation, newPosition);
}

void Planet::rotateAroundOrigin(float angle, float nx, float ny, float nz) {
	rotateAroundOrigin(angle, glm::vec3(nx, ny, nz));
}

void Planet::scale(float scaleFactor) {
	mScale = glm::scale(mScale, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
}

void Planet::setAngularVelocity(const glm::vec3& angularVelocity) {
	mAngularVelocity = angularVelocity;
}

void Planet::setAngularVelocity(float angularVelocity_x, float angularVelocity_y,
                                float angularVelocity_z) {
	mAngularVelocity = glm::vec3(angularVelocity_x, angularVelocity_y, angularVelocity_z);
}

void Planet::setColour(GLfloat r, GLfloat g, GLfloat b) const {
	// Get "setColour" uniform location, enable program, set uniform value, and disable program.
	GLint objColourUniformLocation = mProgram.getUniformLocation("setColour");
	mProgram.enable();
	glUniform3f(objColourUniformLocation, r, g, b);
	mProgram.disable();
}

void Planet::setColour(const std::array<GLfloat, 3>& colour) const {
	setColour(colour[0], colour[1], colour[2]);
}

void Planet::setOpacity(GLfloat alpha) const {
	// Get "setOpacity" uniform location, enable program, set uniform value, and disable program.
	GLint objOpacityUniformLocation = mProgram.getUniformLocation("setOpacity");
	mProgram.enable();
	glUniform1f(objOpacityUniformLocation, alpha);
	mProgram.disable();
}

void Planet::setOrbitalAngularVelocity(const glm::vec3& orbitalAngularVelocity) {
	mOrbitalAngularVelocity = orbitalAngularVelocity;
}

void Planet::setOrbitalAngularVelocity(float orbitalAV_x, float orbitalAV_y, float orbitalAV_z) {
	setOrbitalAngularVelocity(glm::vec3(orbitalAV_x, orbitalAV_y, orbitalAV_z));
}

void Planet::translate(const glm::vec3& displacement) {
	mTranslation = glm::translate(mTranslation, displacement);
}

void Planet::translate(GLfloat x, GLfloat y, GLfloat z) {
	translate(glm::vec3(x, y, z));
}

void Planet::updateState() {
	double currentTime = glfwGetTime();
	float  deltaT      = static_cast<float>(currentTime - mTimeLastStateUpdate);

	// Rotate planet about its own centre according to the planet's angular velocity and time since
	// last state update.
	if (glm::length(mAngularVelocity) > 0.0f) {
		float rotationalAngle = glm::length(mAngularVelocity) * deltaT;
		rotate(rotationalAngle, mAngularVelocity);
	}

	// Rotate cube about the origin according to the cube's orbital angular velocity and the time
	// since the last state update.
	if (glm::length(mOrbitalAngularVelocity) > 0.0f) {
		float orbitalAngle = glm::length(mOrbitalAngularVelocity) * deltaT;
		rotateAroundOrigin(orbitalAngle, mOrbitalAngularVelocity);
	}

	mTimeLastStateUpdate = currentTime;
}

void Planet::draw(const Camera& camera) const {
	// Enable program.
	mProgram.enable();

	// Calculate the model-view-projection matrix and set the corresponding uniform.
	glm::mat4 model      = modelMatrix();
	glm::mat4 view       = camera.view();
	glm::mat4 projection = camera.projection();

	glm::mat4 MVP = projection * view * model;
	glUniformMatrix4fv(mProgram.getUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

	// Bind vertex array object and element buffer object to current context.
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);

	// Draw.
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT,
	               static_cast<GLvoid *>(0));

	// Disable program and unbind vertex array object and element buffer object.
	mProgram.disable();
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}