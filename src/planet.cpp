/**
 * @file planet.cpp
 *
 * Implementation file for the Planet class.
 */
#include "planet.hpp"
#include <glm/glm.hpp>
#include <random>

float colorSTD = 10;
bool Planet::makeRocky = true;

namespace {
// Shader program file paths.
/** Path to vertex shader source code. */
constexpr char VERTEX_SHADER_PATH[]   = "shaders/planet_vertex.shader";
/** Path to fragment shader source code. */
constexpr char FRAGMENT_SHADER_PATH[] = "shaders/planet_fragment.shader";
/** Path to texture asset. */
constexpr char TEXTURES[]             = "textures/rock1.jpg";

// Sphere properties.
/** Number of lines of latitude. */
constexpr unsigned int N_LATITUDE   = 129;
/** Number of lines of longitude. */
constexpr unsigned int N_LONGITUDE  = 129;
/** Smoothness factor*/
float        N_SMOOTHNESS;
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
	// Create the Sphere object which holds the planet's vertex, normal, and index data. Record
	// the number of vertex components and indices.
	if (makeRocky) {
		std::random_device rd;
		std::mt19937       e2(rd());

		std::normal_distribution<float> randomRockiness(1.1f, 0.1f);
		N_SMOOTHNESS = randomRockiness(e2);
	}
	else {
		N_SMOOTHNESS = 5;
	}

	Sphere sphere(1.0f, N_LATITUDE, N_LONGITUDE,
	              N_SMOOTHNESS, makeRocky);//this constructor creates an imperfect sphere for landscape

	mNVertices = static_cast<unsigned int>(sphere.vertices.size());
	mNIndices  = static_cast<unsigned int>(sphere.indices.size());
	mNColors   = static_cast<unsigned int>(sphere.colors.size());

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

	// Set color
	mVertexColors = sphere.colors;

	// Create vertex array buffer, vertex buffer object, and element buffer objects and bind them to
	// current OpenGL context.
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mColorVBO);
	glGenBuffers(1, &mEBO);
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);

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

	// Create and enable vertex attribute for color data.
	glBindBuffer(GL_ARRAY_BUFFER, mColorVBO);
	// Pass color data into array buffer object.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mNColors,
	             static_cast<GLvoid *>(mVertexColors.data()), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
	                      reinterpret_cast<GLvoid *>(0));
	glEnableVertexAttribArray(2);

	// Unbind vertex array buffer, vertex buffer object, and element buffer objects.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

float Planet::size() const {
	return (mScale[0][0] + mScale[1][1] + mScale[2][2]) / 3;
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

bool Planet::planetCollision(const Camera& camera) {

	glm::vec3 cameraPosition = camera.position();
	glm::vec3 camDir         = camera.direction();
	glm::vec3 planetCenter   = position();
	float     planetRadius   = glm::length(mScale * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

	if (glm::length(glm::vec3(cameraPosition.x, cameraPosition.y - 4.0f, cameraPosition.z)
	                + glm::vec3(25 * camDir.x, 25 * camDir.y, 25 * camDir.z) - planetCenter) <=
	    planetRadius * 1.1f) {
		return true;
	}

	return false;


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

	// Set the model uniform.
	glUniformMatrix4fv(mProgram.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

	// Calculate the normal matrix and set the corresponding uniform.
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));
	glUniformMatrix4fv(mProgram.getUniformLocation("normalMatrix"), 1, GL_FALSE,
	                   glm::value_ptr(normalMatrix));

	// Bind vertex array object and element buffer object to current context.
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);

	// Draw.
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mNIndices), GL_UNSIGNED_INT,
	               static_cast<GLvoid *>(0));

	// Disable program and unbind vertex array object and element buffer object.
	mProgram.disable();
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Planet::setPlanetTextureType(TERRAIN_TYPE terrain) {
	if (terrain == ROCKY) {
		setRockyTexture();
	}
	else if (terrain == GASEOUS) {
		setGaseousTexture();
	}
	else if (terrain == EARTH_LIKE) {
		setEarthLikeTexture();
	}
	else {
		std::cerr << "Error: Unknown planet type." << std::endl;
	}

	updateBuffers();
}

void Planet::setRockyTexture() {
	glm::vec3 primaryColor   = getDarkColor();
	glm::vec3 secondaryColor = getDarkColor();

	double xPeriod = 0;
	double yPeriod = 0;

	double turbPower = 100.0;
	double turbSize  = 32.0;

	generateTexture(primaryColor, secondaryColor, xPeriod, yPeriod, turbPower, turbSize);
}

void Planet::setGaseousTexture() {
	glm::vec3 primaryColor   = getBrightColor();
	glm::vec3 secondaryColor = getDarkColor();

	double xPeriod = 5.0;
	double yPeriod = 10.0;

	double turbPower = 10.0;
	double turbSize  = 32.0;

	generateTexture(primaryColor, secondaryColor, xPeriod, yPeriod, turbPower, turbSize);
}

void Planet::setEarthLikeTexture() {
	setRockyTexture();  // Keeping it rocky for now.
	// Might make earth in the future
}

void Planet::generateTexture(glm::vec3 primaryColor, glm::vec3 secondaryColor, double xPeriod,
                             double yPeriod, double turbPower, double turbSize) {
	std::vector<std::vector<GLfloat>> noiseMatrix(N_LATITUDE, std::vector<GLfloat>(N_LONGITUDE));
	noiseMatrix = generateNoiseMatrix();

	// Generate random noise and pattern.
	for (int i = 0; i < N_LATITUDE; i++) {
		for (int j = 0; j < N_LONGITUDE; j++) {
			double xyValue = i * xPeriod / N_LONGITUDE +
			                 j * yPeriod / N_LATITUDE +
			                 turbPower * turbulence(i, j, turbSize, noiseMatrix) / 256.0;

			double sineValue = pow(cos(xyValue * M_PI), 2);

			float factor = static_cast<float>(sineValue);

			mVertexColors[3 * (j + N_LATITUDE * i)]     =
					factor * primaryColor.x + (1 - factor) * secondaryColor.x;
			mVertexColors[3 * (j + N_LATITUDE * i) + 1] =
					factor * primaryColor.y + (1 - factor) * secondaryColor.y;
			mVertexColors[3 * (j + N_LATITUDE * i) + 2] =
					factor * primaryColor.z + (1 - factor) * secondaryColor.z;
		}
	}

}

void Planet::updateBuffers() {
	// Create and enable vertex attribute for color data.
	glBindBuffer(GL_ARRAY_BUFFER, mColorVBO);
	// Pass color data into array buffer object.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mNColors,
	             static_cast<GLvoid *>(mVertexColors.data()), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
	                      reinterpret_cast<GLvoid *>(0));
	glEnableVertexAttribArray(2);
}

std::vector<std::vector<GLfloat>> generateNoiseMatrix() {
	std::random_device rd;
	std::mt19937       e2(rd());

// Generate random noise
	std::uniform_real_distribution<float> randomNoise(0.0f, 1.0f);

	std::vector<std::vector<GLfloat>> noiseMatrix(N_LATITUDE, std::vector<GLfloat>(N_LONGITUDE));

// Create noise
	for (int i = 0; i < N_LATITUDE; i++) {
		for (int j = 0; j < N_LONGITUDE; j++) {
			noiseMatrix[i][j] = randomNoise(e2);
		}
	}

	return noiseMatrix;
}

// This code was made with the help of http://lodev.org/cgtutor/randomnoise.html
float smoothNoise(float x, float y, const std::vector<std::vector<GLfloat>>& noise) {
	//get fractional part of x and y
	double fractX = x - floor(x);
	double fractY = y - floor(y);

	//wrap around
	int x1 = (static_cast<int>(x) + N_LONGITUDE) % N_LONGITUDE;
	int y1 = (static_cast<int>(y) + N_LATITUDE) % N_LATITUDE;

	//neighbor values
	int x2 = (x1 + N_LONGITUDE - 1) % N_LONGITUDE;
	int y2 = (y1 + N_LATITUDE - 1) % N_LATITUDE;

	//smooth the noise with bilinear interpolation
	float value = 0.0;
	value += fractX * fractY * noise[y1][x1];
	value += (1 - fractX) * fractY * noise[y1][x2];
	value += fractX * (1 - fractY) * noise[y2][x1];
	value += (1 - fractX) * (1 - fractY) * noise[y2][x2];

	return value;
}

// This code was made with the help of http://lodev.org/cgtutor/randomnoise.html
double turbulence(double x, double y, double size, const std::vector<std::vector<GLfloat>>& noise) {
	double value = 0.0, initialSize = size;

	while (size >= 1) {
		value += smoothNoise(x / size, y / size, noise) * size;
		size /= 2.0;
	}

	return (128.0 * value / initialSize);
}

glm::vec3 getBrightColor() {
	std::random_device rd;
	std::mt19937       e2(rd());

	// Pick a random color

	glm::vec3 colorChoice;

	std::vector<glm::vec3>             colorChoices {
			glm::vec3 { 102, 204, 255 },
			glm::vec3{ 236, 121, 154 },
			glm::vec3{ 224, 204, 151 },
			glm::vec3{ 225, 179, 120 },
			glm::vec3{ 244, 170, 262 }
	};
	std::uniform_int_distribution<int> randomColor(0, colorChoices.size() - 1);

	colorChoice = colorChoices[randomColor(e2)];

	std::normal_distribution<float> randomR(colorChoice.x, colorSTD);
	std::normal_distribution<float> randomG(colorChoice.y, colorSTD);
	std::normal_distribution<float> randomB(colorChoice.z, colorSTD);

	return glm::vec3{ randomR(e2) / 255, randomG(e2) / 255, randomB(e2) / 255 };
}

// Same code as above except for dark colors
glm::vec3 getDarkColor() {
	std::random_device rd;
	std::mt19937       e2(rd());

	// Pick a random color

	glm::vec3 colorChoice;

	std::vector<glm::vec3>             colorChoices {
			glm::vec3{ 121, 72, 59 },
			glm::vec3{ 73, 99, 141 },
			glm::vec3{ 184, 41, 30 },
			glm::vec3{ 56, 132, 65 },
			glm::vec3{ 70, 117, 118 }
	};
	std::uniform_int_distribution<int> randomColor(0, colorChoices.size() - 1);

	colorChoice = colorChoices[randomColor(e2)];

	std::normal_distribution<float> randomR(colorChoice.x, colorSTD);
	std::normal_distribution<float> randomG(colorChoice.y, colorSTD);
	std::normal_distribution<float> randomB(colorChoice.z, colorSTD);

	return glm::vec3{ randomR(e2) / 255, randomG(e2) / 255, randomB(e2) / 255 };
}
