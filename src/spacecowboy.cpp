/**
* @file spacecowboy.cpp
*
* Implementation file for the spacecowboy class.
*/
#include "spacecowboy.hpp"

namespace {
	// spacecowboy file paths.
	/** Path to vertex shader source code. */
	constexpr char VERTEX_SHADER_PATH[] = "shaders/spacecowboy_vertex.shader";
	/** Path to fragment shader source code. */
	constexpr char FRAGMENT_SHADER_PATH[] = "shaders/spacecowboy_fragment.shader";
	/** Path to object files. */
	constexpr char SPACECOWBOY_ALIEN_OBJ[] = "assets/Alien.obj";
	constexpr char SPACECOWBOY_DEADPOOL_OBJ[] = "assets/Deadpool/DeadPool.obj";
	constexpr char SPACECOWBOY_DEADPOOL_PNG[] = "assets/Deadpool/deadpool_tex01_bm.png";
}

// Constructors.
Spacecowboy::Spacecowboy() :
	mProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH),
	mPosition(),
	mScale(),
	mRotation(),
	mTranslation(),
	mVelocity(),
	mTimeLastFrame(glfwGetTime()) {

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;

	loadOBJ(SPACECOWBOY_DEADPOOL_OBJ, vertices, normals, UVs);

	// Create vertex array buffer, vertex buffer object, and element buffer objects and bind them to
	// current OpenGL context.

	mNVertices = static_cast<unsigned int>(vertices.size());

	//Generate all needed IDs
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mUV_VBO);
	glGenBuffers(1, &mN_VBO);

	//Bind the VAO
	glBindVertexArray(mVAO);

	// Bind and buffer the Vertices into the VBO and enable position 0
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
	glEnableVertexAttribArray(0);

	// Bind and buffer the normals into the normal VBO and put at location 1
	glBindBuffer(GL_ARRAY_BUFFER, mN_VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * normals.size(), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
	glEnableVertexAttribArray(1);


	// Bind and buffer the UVs into the UV VBO at location 2
	glBindBuffer(GL_ARRAY_BUFFER, mUV_VBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * UVs.size() * sizeof(GLfloat), &UVs.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
	glEnableVertexAttribArray(2);

	//Unbind everything for safety
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//generate texture here
	glActiveTexture(GL_TEXTURE0); //select texture unit 1

	glGenTextures(1, &spacecowboy_texture);
	glBindTexture(GL_TEXTURE_2D, spacecowboy_texture); //bind this texture to the currently bound texture unit

	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load image, create texture and generate mipmaps
	int spacecowboy_texture_width, spacecowboy_texture_height;
	unsigned char* spacecowboy_image = SOIL_load_image(SPACECOWBOY_DEADPOOL_PNG, &spacecowboy_texture_width, &spacecowboy_texture_height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, spacecowboy_texture_width, spacecowboy_texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, spacecowboy_image);

	SOIL_free_image_data(spacecowboy_image); //free resources

	//unbind for safety
	glBindTexture(GL_TEXTURE_2D, 0);

	//setColour(palette::RED);
	setOpacity(palette::OPAQUE);

	scale(0.1);

}

// Accessor functions.
glm::vec3 Spacecowboy::position() const {
	return glm::vec3(mTranslation[3]);
}

glm::vec3 Spacecowboy::velocity() const {
	return mVelocity;
}

glm::mat4 Spacecowboy::modelMatrix() const {
	return mRotation * mTranslation * mScale;
}

// Mutator functions.
void Spacecowboy::setVelocity(const glm::vec3& velocity) {
	mVelocity = velocity;
}

void Spacecowboy::translate(const glm::vec3& displacement) {
	mTranslation = glm::translate(mTranslation, displacement);
}

void Spacecowboy::updateState(const Camera& camera) {
	double currentTime = glfwGetTime();
	float  deltaT = static_cast<float>(currentTime - mTimeLastFrame);

	glm::vec3 oldPosition = position();

	glm::vec3 camDir = camera.direction();
	glm::vec3 camPos = camera.position();

	glm::vec3 newPosition = glm::vec3(camPos.x, camPos.y, camPos.z) + glm::vec3(20 * camDir.x, 20 * camDir.y, 20 * camDir.z);
	mTranslation = glm::translate(mTranslation, -oldPosition);
	mTranslation = glm::translate(mTranslation, newPosition);


	mTimeLastFrame = currentTime;
}

void Spacecowboy::setColour(GLfloat r, GLfloat g, GLfloat b) const {
	// Get "objectColour" uniform location, enable program, set uniform value, and disable program.
	GLint objectColourUniformLocation = mProgram.getUniformLocation("objectColour");
	mProgram.enable();
	glUniform3f(objectColourUniformLocation, r, g, b);
	mProgram.disable();
}

void Spacecowboy::setColour(const std::array<GLfloat, 3>& colour) const {
	setColour(colour[0], colour[1], colour[2]);
}

void Spacecowboy::setOpacity(GLfloat alpha) const {
	// Get "objectOpacity" uniform location, enable program, set uniform value, and disable program.
	GLint objectOpacityUniformLocation = mProgram.getUniformLocation("objectOpacity");
	mProgram.enable();
	glUniform1f(objectOpacityUniformLocation, alpha);
	mProgram.disable();
}

void Spacecowboy::setPosition(const glm::vec3& position) {
	mPosition = position;
}

void Spacecowboy::scale(float scaleFactor) {
	mScale = glm::scale(mScale, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
}

void Spacecowboy::draw(const Camera& camera) const
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

	glUniform1i(mProgram.getUniformLocation("spacecowboyTexture"), 0); //tell our uniform texture sampler to sample texture unit 2

	// Bind vertex array object and element buffer object to current context.
	glBindVertexArray(mVAO);
	//glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, spacecowboy_texture); 

	// Draw.

	glDrawArrays(GL_TRIANGLES, 0, mNVertices);

	

	// Disable program and unbind vertex array object and element buffer object.
	mProgram.disable();
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}