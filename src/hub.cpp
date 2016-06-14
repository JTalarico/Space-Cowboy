
#include "hub.hpp"
#include <glm/glm.hpp>

namespace {
	// Shader program file paths.
	/** Path to vertex shader source code. */
	constexpr char VERTEX_SHADER_PATH[] = "shaders/hub_vertex.shader";
	/** Path to fragment shader source code. */
	constexpr char FRAGMENT_SHADER_PATH[] = "shaders/hub_fragment.shader";
	/** Path to texture asset. */
	constexpr char TEXTURES[] = "textures/window.png";

}



// Constructors.
Hub::Hub() :
	mProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH){
	
	
	//vertices of window
	//in NDC
	 windowVertices = {
	-0.5f, 0.5f, 0.0f, //top-left vert 
	0.5f, 0.5f, 0.0f, //top-right vert
	0.5f, -0.5f, 0.0f, //bottom-right vert
	-0.5f, -0.5f, 0.0f, //bottom-left vert
	};

	windowUVs = {
	0.0f, 1.0f, //top-left vert
	1.0f, 1.0f, //top-right vert
	1.0f, 0.0f, //bottom-right vert
	0.0f, 0.0f, //bottom-left vert
	};

	windowIndices= {
	3, 0, 2,
	0, 1, 2
	};

//window VAO
glGenVertexArrays(1, &windowVAO);
glBindVertexArray(windowVAO);

//vbo for window vertices
glGenBuffers(1, &windowVerticesVBO);
glBindBuffer(GL_ARRAY_BUFFER, windowVerticesVBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(windowVertices), &windowVertices.front(), GL_STATIC_DRAW);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
glEnableVertexAttribArray(0);

//vbo for window UVs
glGenBuffers(1, &windowUVsVBO);
glBindBuffer(GL_ARRAY_BUFFER, windowUVsVBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(windowUVs), &windowUVs.front(), GL_STATIC_DRAW);
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
glEnableVertexAttribArray(1);

//indices (EBO) for window
glGenBuffers(1, &windowEBO);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, windowEBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(windowIndices), &windowIndices.front(), GL_STATIC_DRAW);

glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);

	//Texture HUB

	//Window texture
glActiveTexture(GL_TEXTURE4); //select texture unit 4

glGenTextures(1, &hub_Texture);
glBindTexture(GL_TEXTURE_2D, hub_Texture); //bind this texture to the currently bound texture unit

// Load image, create texture and generate mipmaps
int window_texture_width, window_texture_height;
unsigned char* window_texture_image = SOIL_load_image(TEXTURES, &window_texture_width, &window_texture_height, 0, SOIL_LOAD_RGBA);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_texture_width, window_texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, window_texture_image);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

SOIL_free_image_data(window_texture_image); //free resources



}

//--------------------------------------------------  DRAW METHOD ----------------------------------------------------------------

void Hub::draw(const Camera& camera) const {
	// Enable program.
	mProgram.enable();
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glUniform1i(mProgram.getUniformLocation("hubTexture"), 4); //tell our uniform texture sampler to sample texture unit 4

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = camera.view();
	glm::mat4 projection = camera.projection();

	glm::mat4 MVP = projection * view * model;
	glUniformMatrix4fv(mProgram.getUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

	// Bind vertex array object and element buffer object to current context.
	glBindVertexArray(windowVAO);
	glBindBuffer(GL_ARRAY_BUFFER, windowUVsVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, windowEBO);

	glBindTexture(GL_TEXTURE_2D, hub_Texture);

	// Draw.
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
		0);

	// Disable program and unbind vertex array object and element buffer object.
	mProgram.disable();
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
