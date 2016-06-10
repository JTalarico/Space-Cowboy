/**
 * @file stars.cpp
 *
 * Implementation file for the stars class.
 */
#include "stars.hpp"

namespace {
/** Array of file paths to the texture image for each of the skybox's faces in the following order:
 *  right, left, up, down, back, front. */
const std::array<std::string, 6> TEXTURE_FACES = { "assets/purplenebula_rt.png",
                                                   "assets/purplenebula_lf.png",
                                                   "assets/purplenebula_up.png",
                                                   "assets/purplenebula_dn.png",
                                                   "assets/purplenebula_bk.png",
                                                   "assets/purplenebula_ft.png" };

/** Path to vertex shader source code. */
constexpr char VERTEX_SHADER_PATH[]   = "shaders/stars_vertex.shader";
/** Path to fragment shader source code. */
constexpr char FRAGMENT_SHADER_PATH[] = "shaders/stars_fragment.shader";
}

// Redeclare constant static data members.
constexpr std::array<GLfloat, 108> Stars::sVertices;

// Constructors.
Stars::Stars() :
		mTextureID(createTexture()),
		mProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) {
	// Create vertex array buffer and vertex buffer object and bind them to current OpenGL context.
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	// Pass vertex data into vertex buffer object.
	glBufferData(GL_ARRAY_BUFFER, sizeof(sVertices), sVertices.data(), GL_STATIC_DRAW);

	// Create and enable vertex attribute for vertex data.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
	                      reinterpret_cast<GLvoid *>(0));
	glEnableVertexAttribArray(0);

	// Unbind vertex array buffer and vertex buffer object.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Stars::draw(const Camera& camera) const {
	// Disable depth writing so that skybox appears as background.
	glDepthMask(GL_FALSE);

	// Enable shader program.
	mProgram.enable();

	// Set view and projection matrices.
	glm::mat4 projection = camera.projection();
	glm::mat4 view       = glm::mat4(glm::mat3(camera.view()));
	glUniformMatrix4fv(mProgram.getUniformLocation("projection"), 1, GL_FALSE,
	                   glm::value_ptr(projection));
	glUniformMatrix4fv(mProgram.getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));

	// Bind the vertex array buffer and texture to current context.
	glBindVertexArray(mVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);

	// Draw.
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Unbind vertex array buffer and texture and re-enable depth writing. Disable program.
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDepthMask(GL_TRUE);
	mProgram.disable();
}

// Helper functions.
GLuint Stars::createTexture() {
	// Generate texture ID for cubemap and bind it to the current OpenGL context.
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	// Load image for each face and bind to cubemap texture. Throw exception if image cannot be
	// loaded.
	for (GLuint i = 0; i < TEXTURE_FACES.size(); ++i) {
		int           texture_width, texture_height;
		unsigned char *image;

		image = SOIL_load_image(TEXTURE_FACES[i].c_str(), &texture_width, &texture_height, 0,
		                        SOIL_LOAD_RGB);
		if (image == nullptr) {
			throw std::runtime_error(
					"Failed to locate image " + std::string(TEXTURE_FACES[i]) + ".");
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, texture_width, texture_height,
		             0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	// Set texture parameters.
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Unbind texture.
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}
