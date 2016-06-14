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

	scale(0.2f);
	tiltDegree = 0.0f;
	tiltRotation = glm::mat4(1.0f);

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

	// Position the ship in front of the camera
	glm::vec3 oldPosition = position();

	glm::vec3 camDir = camera.direction();
	glm::vec3 camPos = camera.position();

	glm::vec3 newPosition = glm::vec3(camPos.x, camPos.y - 4.0f, camPos.z) + glm::vec3(25 * camDir.x, 25 * camDir.y, 25 * camDir.z);
	mTranslation = glm::translate(mTranslation, -oldPosition);
	mTranslation = glm::translate(mTranslation, newPosition);

	// Rotate the ship so it points in the camera direction

	pointNose(camera);

}

void Spaceship::pointNose(const Camera& camera) {

	// The space ship is loaded pointing in the negative X direction due to object File
	glm::vec3 spaceShipDir = glm::vec3(-1.0f, 0.0f, 0.0f);

	//Useful Values variable that will come up often
	glm::vec3 camDir = camera.direction();
	glm::mat4 identity = glm::mat4(1.0f);

	// Our first goal is to find the rotation on the XZ plane, to do so we map the camera direction to the xz plane and find the angle theta

	float xzDot = glm::dot(spaceShipDir, glm::vec3(camDir.x, 0.0f, camDir.z));
	float xzLength = glm::length(glm::vec3(camDir.x, 0.0f, camDir.z));
	float theta = glm::acos(xzDot / xzLength);

	// Next we can define the first rotation realising the dot product will never let us rotate more than 180 degrees, we adjust for the
	// two halves of the xz plane
	glm::mat4 rotation1;
	if (camDir.z < 0)
		rotation1 = glm::rotate(identity, theta, glm::vec3(0.0f, -1.0f, 0.0f));
	else
		rotation1 = glm::rotate(identity, theta, glm::vec3(0.0f, 1.0f, 0.0f));

	// We find the angle between the xz plane and the camera direction using the dot product between the camera direction and its mapping on the xz plane
	// The axis of rotation will be the cross product between the two
	glm::vec3 xzProjection = glm::vec3(camDir.x, 0.0f, camDir.z);
	glm::vec3 rotationAxis = glm::cross(camDir, xzProjection);

	// Similar to above
	float dot = glm::dot(camDir, xzProjection);
	float xzpLength = glm::length(xzProjection);
	float camLength = glm::length(camDir);
	float phi = glm::acos(dot / (xzpLength*camLength));

	// Adjust the rotation accordingly
	glm::mat4 rotation2;
	if (camDir.y > 0)
		rotation2 = glm::rotate(identity, phi, -rotationAxis);
	else
		rotation2 = glm::rotate(identity, -phi, rotationAxis);

	std::cout << camera.getTiltLeft() << std::endl;
	// The rotation matrix needed for the model is the combination of these two Rotations

	
	if (camera.getTiltLeft()) {
		
		if (tiltDegree >= -3.14159f / 2.0f) 
			tiltDegree -= 0.1f;

		tiltRotation = glm::rotate(identity, tiltDegree, camDir);
				
	}
	
	else if (camera.getTiltRight()) {

		if (tiltDegree <= 3.14159f / 2.0f)
			tiltDegree += 0.1f;

		tiltRotation = glm::rotate(identity, tiltDegree, camDir);
	}
	else if (!(camera.getTiltLeft() || camera.getTiltRight())) {

		if (tiltDegree != 0)
			tiltDegree *= 0.9f;
		
		tiltRotation = glm::rotate(identity, tiltDegree, camDir);

	}else{
	
	}

	mRotation = tiltRotation * rotation2 * rotation1;

	//---------------------Here an attempt was made for Tilt. -----------------------
	/*
	//Here we will add tilt to the glide of the ship

	glm::vec3 oldXZ = glm::vec3(mOldCamDir.x, 0.0f, mOldCamDir.z);

	float tiltDot = glm::dot(xzProjection, oldXZ);
	float oldLength = glm::length(oldXZ);

	float psi = glm::acos(tiltDot / (xzpLength*oldLength));

	glm::mat4 rotation3 = glm::rotate(identity, psi, -camDir);

	mRotation = rotation3 * rotation2 * rotation1;

	std::cout << psi << std::endl;

	mOldCamDir = camDir;
	*/

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