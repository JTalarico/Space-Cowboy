/**
* @file planet.cpp
*
* Implementation file for the Planet class.
*/
#include "planet.hpp"
#include <glm/glm.hpp>

namespace {
	// Shader program file paths.
	/** Path to vertex shader source code. */
	constexpr char VERTEX_SHADER_PATH[] = "shaders/planet_vertex.shader";
	/** Path to fragment shader source code. */
	constexpr char FRAGMENT_SHADER_PATH[] = "shaders/planet_fragment.shader";
	/** Path to texture asset. */
	constexpr char TEXTURES[] = "textures/rock1.jpg";

	// Sphere properties.
	/** Number of lines of latitude. */
	constexpr unsigned int N_LATITUDE = 129;
	/** Number of lines of longitude. */
	constexpr unsigned int N_LONGITUDE = 129;
	/** Smoothness factor*/
	constexpr float N_SMOOTHNESS = 1.2f;
}

GLuint sphere_texture;

// Constructors.
Planet::Planet() :
	mProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH),
	mScale(),
	mRotation(),
	mTranslation(),
	mAngularVelocity(),
	mOrbitalAngularVelocity(),
	mTimeLastStateUpdate(glfwGetTime()) {
	// Create the Sphere object which holds the planet's vertex, normal, and index data. Record
	// the number of vertex components and indices.
	Sphere sphere(1.0f, N_LATITUDE, N_LONGITUDE, N_SMOOTHNESS);//this constructor creates an imperfect sphere for landscape
	mNVertices = static_cast<unsigned int>(sphere.vertices.size());
	mNIndices = static_cast<unsigned int>(sphere.indices.size());

	// Combine the vertices and normals into a single vector such that each triplet of vertex
	// components is followed by the components of the vertex's normal.
	std::vector<GLfloat> vertexBufferData;
	vertexBufferData.reserve(2 * mNVertices);

	for (unsigned int i = 0; i < mNVertices; i += 3) {
		// Vertex data.
		vertexBufferData.push_back(sphere.vertices[i]);
		vertexBufferData.push_back(sphere.vertices[i + 1]);
		vertexBufferData.push_back(sphere.vertices[i + 2]);

		// Normal data.
		vertexBufferData.push_back(sphere.normals[i]);
		vertexBufferData.push_back(sphere.normals[i + 1]);
		vertexBufferData.push_back(sphere.normals[i + 2]);

	}

	// Create vertex array buffer, vertex buffer object, and element buffer objects and bind them to
	// current OpenGL context.
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);

	//create uv buffer object and add data to it give in location of 2 in shaders
	glGenBuffers(1, &mUV_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, mUV_VBO);
	glBufferData(GL_ARRAY_BUFFER, sphere.uvs.size() * sizeof(GLfloat), &sphere.uvs.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
	glEnableVertexAttribArray(2);


	// Pass vertex and normal data into vertex buffer object.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexBufferData.size(),
		static_cast<GLvoid *>(vertexBufferData.data()), GL_STATIC_DRAW);

	// Pass index data into element buffer object.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mNIndices,
		static_cast<GLvoid *>(sphere.indices.data()), GL_STATIC_DRAW);

	// Create and enable vertex attribute for vertex data.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
		reinterpret_cast<GLvoid *>(0));
	glEnableVertexAttribArray(0);

	// Create and enable vertex attribute for normal data.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
		reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Unbind vertex array buffer, vertex buffer object, and element buffer objects.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	sphere.textureSphere(TEXTURES, sphere_texture);

	// Set colour and opacity.
	//setColour(palette::BLUE);
	//setOpacity(palette::OPAQUE);
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
	return mTranslation * mRotation * mScale;
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
	glm::vec3 oldPosition = position();

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
	// Get "objectColour" uniform location, enable program, set uniform value, and disable program.
	GLint objectColourUniformLocation = mProgram.getUniformLocation("objectColour");
	mProgram.enable();
	glUniform3f(objectColourUniformLocation, r, g, b);
	mProgram.disable();
}

void Planet::setColour(const std::array<GLfloat, 3>& colour) const {
	setColour(colour[0], colour[1], colour[2]);
}

void Planet::setOpacity(GLfloat alpha) const {
	// Get "objectOpacity" uniform location, enable program, set uniform value, and disable program.
	GLint objectOpacityUniformLocation = mProgram.getUniformLocation("objectOpacity");
	mProgram.enable();
	glUniform1f(objectOpacityUniformLocation, alpha);
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
	float  deltaT = static_cast<float>(currentTime - mTimeLastStateUpdate);

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

// TEMPLATE : glm::vec3(camPos.x, camPos.y - 4.0f, camPos.z) + glm::vec3(25 * camDir.x, 25 * camDir.y, 25 * camDir.z)

bool Planet::planetCollision(const Camera &camera) {

	glm::vec3 cameraPosition = camera.position();
	glm::vec3 camDir = camera.direction();
	glm::vec3 planetCenter = position();
	float planetRadius = glm::length(mScale*glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

	if (glm::length(glm::vec3(cameraPosition.x, cameraPosition.y - 4.0f, cameraPosition.z)
		+ glm::vec3(25 * camDir.x, 25 * camDir.y, 25 * camDir.z) - planetCenter) <= planetRadius*1.1f) {
		return true;
	}

	return false;


}
void Planet::draw(const Camera& camera) const {
	// Enable program.
	mProgram.enable();

	// Calculate the model-view-projection matrix and set the corresponding uniform.
	glm::mat4 model = modelMatrix();
	glm::mat4 view = camera.view();
	glm::mat4 projection = camera.projection();

	glm::mat4 MVP = projection * view * model;
	glUniformMatrix4fv(mProgram.getUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

	// Set the model uniform.
	glUniformMatrix4fv(mProgram.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

	// Calculate the normal matrix and set the corresponding uniform.
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));
	glUniformMatrix4fv(mProgram.getUniformLocation("normalMatrix"), 1, GL_FALSE,
		glm::value_ptr(normalMatrix));

	
	glUniform1i(mProgram.getUniformLocation("planetTexture"), 0); //tell our uniform texture sampler to sample texture unit 0

																  // Bind vertex array object and element buffer object to current context.
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mUV_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);

	glBindTexture(GL_TEXTURE_2D, sphere_texture);

	// Draw.
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mNIndices), GL_UNSIGNED_INT,
		static_cast<GLvoid *>(0));

	// Disable program and unbind vertex array object and element buffer object.
	mProgram.disable();
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}