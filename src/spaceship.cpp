/**
 * @file spaceship.cpp
 *
 * Implementation file for the Spaceship class.
 */
#include "spaceship.hpp"
#include <iostream>
namespace {
	// Spaceship file paths.
	/** Path to vertex shader source code. */
	constexpr char VERTEX_SHADER_PATH[] = "shaders/spaceship_vertex.shader";
	/** Path to fragment shader source code. */
	constexpr char FRAGMENT_SHADER_PATH[] = "shaders/spaceship_fragment.shader";
	/** Path to object files. */
	constexpr char AERO4_FILE[] = "assets/aero4.obj"; 
	constexpr char DARK_FIGHTER_6[] = "assets/dark_fighter_6/dark_fighter_6.obj";
}

// Constructors.
Spaceship::Spaceship() :
		mProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH),
		mPosition(),
		mScale(),
		mRotation(),
		mTranslation(),
		mVelocity(),
		mOldCamDir(),
		mNewCamDir(),
		mTimeLastFrame(glfwGetTime()) { 
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;
	
	mOldCamDir = glm::vec3(0.0f, 0.0f, -1.0f);
	mRotation = glm::rotate(glm::mat4(1.0f), -3.14159f / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	loadOBJ(DARK_FIGHTER_6, vertices, normals, UVs);

	mNVertices = static_cast<unsigned int>(vertices.size());


	// Create vertex array buffer, vertex buffer object, and element buffer objects and bind them to
	// current OpenGL context.
	
	
	//Generate all needed IDs
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mUV_VBO);
	glGenBuffers(1, &mN_VBO);
	
	//Bind the VAO
	glBindVertexArray(mVAO);
	
	// Bind and buffer the Vertices into the VBO and enable position 0
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, 3*sizeof(GLfloat) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),reinterpret_cast<GLvoid *>(0));
	glEnableVertexAttribArray(0);

	// Bind and buffer the normals into the normal VBO and put at location 1
	glBindBuffer(GL_ARRAY_BUFFER, mN_VBO);
	glBufferData(GL_ARRAY_BUFFER, 3*sizeof(GLfloat) * normals.size(), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
	glEnableVertexAttribArray(1);


	// Bind and buffer the UVs into the UV VBO at location 2
	glBindBuffer(GL_ARRAY_BUFFER, mUV_VBO);
	glBufferData(GL_ARRAY_BUFFER, 2*UVs.size() * sizeof(GLfloat), &UVs.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
	glEnableVertexAttribArray(2);
		
	//Unbind everything for safety
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//generate texture here
	glActiveTexture(GL_TEXTURE0); //select texture unit 0

	glGenTextures(1, &ship_texture);
	glBindTexture(GL_TEXTURE_2D, ship_texture); //bind this texture to the currently bound texture unit

												// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load image, create texture and generate mipmaps
	int ship_texture_width, ship_texture_height;
	unsigned char* ship_image = SOIL_load_image("assets/dark_fighter_6/dark_fighter_6_color.png", &ship_texture_width, &ship_texture_height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ship_texture_width, ship_texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, ship_image);

	SOIL_free_image_data(ship_image); //free resources
	
	//setColour(palette::BLUE);
	setOpacity(palette::OPAQUE);

}

// Accessor functions.
glm::vec3 Spaceship::position() const {
	return glm::vec3(mTranslation[3]);
}

glm::vec3 Spaceship::velocity() const {
	return mVelocity;
}

glm::mat4 Spaceship::modelMatrix() const {
	return  mTranslation *mRotation* mScale;
}

// Mutator functions.
void Spaceship::setVelocity(const glm::vec3& velocity) {
	mVelocity = velocity;
}

void Spaceship::translate(const glm::vec3& displacement) {
	mTranslation = glm::translate(mTranslation, displacement);
}

void Spaceship::updateState(const Camera& camera) {
	double currentTime = glfwGetTime();
	float  deltaT      = static_cast<float>(currentTime - mTimeLastFrame);
	glm::vec3 oldPosition = position();//should get current pos
	
	glm::vec3 camDir = camera.direction();
	glm::vec3 camPos = camera.position();
	
	glm::vec3 newPosition = glm::vec3(camPos.x,camPos.y-8.0f, camPos.z) + glm::vec3(80*camDir.x,80*camDir.y, 80*camDir.z);
	mTranslation = glm::translate(mTranslation, -oldPosition);
	mTranslation = glm::translate(mTranslation, newPosition);

	float theta = glm::acos(glm::dot(camDir, mOldCamDir)/(glm::length(camDir))/glm::length(mOldCamDir));

	glm::vec3 axisOfRotation = glm::cross(camDir, mOldCamDir);
	
				

	if (camDir != mOldCamDir) {
		
		std::cout << theta << std::endl;
		
		mRotation *= glm::rotate(glm::mat4(1.0f), theta, -axisOfRotation);
	}


	mOldCamDir = camDir;

	mTimeLastFrame = currentTime;
}

void Spaceship::rotateShip()
{
	mRotation = glm::rotate(glm::mat4(1.0f), -3.14159f / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
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

void Spaceship::setPosition(const glm::vec3& position) {
	mPosition = position;
}

void Spaceship::scale(float scaleFactor) {
	mScale = glm::scale(mScale, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
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

	glUniform1i(mProgram.getUniformLocation("shipTexture"), 0); //tell our uniform texture sampler to sample texture unit 0

// Bind vertex array object and element buffer object to current context.
	glBindVertexArray(mVAO);

	glBindTexture(GL_TEXTURE_2D, ship_texture); // replace with spaceships texture, create function?
	

	// Draw.
	//glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mNIndices), GL_UNSIGNED_INT,static_cast<GLvoid *>(0));
	glDrawArrays(GL_TRIANGLES, 0, mNVertices);

	
	// Disable program and unbind vertex array object and element buffer object.
	mProgram.disable();
	glBindVertexArray(0);
	
	
}