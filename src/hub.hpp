/**
* @file planet.hpp
*
* Interface file for the Planet class.
*/
#ifndef SPACE_COWBOY_HUB_HPP
#define SPACE_COWBOY_HUB_HPP

#include "camera.hpp"
#include "palette.hpp"
#include "program.hpp"

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <soil/SOIL.h>


class Hub {
public:
	// Constructors.
	/**
	* Creates a unit sphere centred at the origin.
	*/
	Hub();


	void draw(const Camera& camera) const;


private:
	// Data members.
	/** Shader program. */
	Program mProgram;
	/** Reference ID of vertex array buffer. */
	GLuint  windowVAO;
	/** Reference ID of vertex buffer object. */
	GLuint  windowVerticesVBO;
	/** Reference ID of element buffer object. */
	GLuint  windowEBO;

	/** Reference ID of vertex uv buffer. */
	GLuint  windowUVsVBO;

	/*Reference ID of hud texture*/
	GLuint hub_Texture;
	
	std::vector<GLfloat> windowVertices;

	std::vector<GLfloat> windowUVs;

	std::vector<GLuint> windowIndices;
	
	
};

#endif
