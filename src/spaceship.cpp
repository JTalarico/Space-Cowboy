/**
 * @file spaceship.cpp
 *
 * Implementation file for the Spaceship class.
 */
#include "spaceship.hpp"

namespace {
	// Spaceship file paths.
	/** Path to vertex shader source code. */
	constexpr char VERTEX_SHADER_PATH[] = "shaders/spaceship_vertex.shader";
	/** Path to fragment shader source code. */
	constexpr char FRAGMENT_SHADER_PATH[] = "shaders/spaceship_fragment.shader";
	/** Path to object files. */
	constexpr char AERO4_FILE[] = "assets/aero4.obj"; 
	constexpr char DARK_FIGHTER_6[] = "assets/dark_fighter_6.obj";
}

// Constructors.
Spaceship::Spaceship() :
		mProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH),
		mPosition(),
		mVelocity(),
		mTimeLastFrame(glfwGetTime()) { 
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;

	loadOBJ(DARK_FIGHTER_6, vertices, normals, UVs);

	mNVertices = static_cast<unsigned int>(vertices.size());


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
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(GLfloat), &UVs.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
	glEnableVertexAttribArray(2);

	// Pass vertex data into vertex buffer object.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(),
		static_cast<GLvoid *>(vertices.data()), GL_STATIC_DRAW);

	// Create and enable vertex attribute for vertex data.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
		reinterpret_cast<GLvoid *>(0));
	glEnableVertexAttribArray(0);

	// Pass vertex data into vertex buffer object.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normals.size(),
		static_cast<GLvoid *>(normals.data()), GL_STATIC_DRAW);

	// Create and enable vertex attribute for normal data.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
		reinterpret_cast<GLvoid *>(0));
	glEnableVertexAttribArray(1);

	// Unbind vertex array buffer, vertex buffer object, and element buffer objects.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	setColour(palette::RED);
	setOpacity(palette::OPAQUE);

}

// Accessor functions.
glm::vec3 Spaceship::position() const {
	return mPosition;
}

glm::vec3 Spaceship::velocity() const {
	return mVelocity;
}

glm::mat4 Spaceship::modelMatrix() const {
	return mRotation * mTranslation * mScale;
}

// Mutator functions.
void Spaceship::setVelocity(const glm::vec3& velocity) {
	mVelocity = velocity;
}

void Spaceship::translate(const glm::vec3& displacement) {
	mPosition += displacement;
}

void Spaceship::updateState() {
	double currentTime = glfwGetTime();
	float  deltaT      = static_cast<float>(currentTime - mTimeLastFrame);

	mPosition += deltaT * mVelocity;

	mTimeLastFrame = currentTime;
}

void Spaceship::setColour(GLfloat r, GLfloat g, GLfloat b) const {
	// Get "objectColour" uniform location, enable program, set uniform value, and disable program.
	GLint objectColourUniformLocation = mProgram.getUniformLocation("objectColour");
	mProgram.enable();
	glUniform3f(objectColourUniformLocation, r, g, b);
	mProgram.disable();
}

void Spaceship::setColour(const std::array<GLfloat, 3>& colour) const {
	setColour(colour[0], colour[1], colour[2]);
}

void Spaceship::setOpacity(GLfloat alpha) const {
	// Get "objectOpacity" uniform location, enable program, set uniform value, and disable program.
	GLint objectOpacityUniformLocation = mProgram.getUniformLocation("objectOpacity");
	mProgram.enable();
	glUniform1f(objectOpacityUniformLocation, alpha);
	mProgram.disable();
}

void Spaceship::draw(const Camera& camera) const
{
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

	//glUniform1i(mProgram.getUniformLocation("planetTexture"), 0); //tell our uniform texture sampler to sample texture unit 0

// Bind vertex array object and element buffer object to current context.
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mUV_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);

	//glBindTexture(GL_TEXTURE_2D, sphere_texture); // replace with spaceships texture, create function?

	// Draw.
	//glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mNIndices), GL_UNSIGNED_INT,static_cast<GLvoid *>(0));
	glDrawArrays(GL_TRIANGLES, 0, mNVertices);

	// Disable program and unbind vertex array object and element buffer object.
	mProgram.disable();
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}