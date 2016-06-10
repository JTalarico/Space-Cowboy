/**
* @file objloader.hpp
*
* Interface file for object loading. Taken from lab 8.
*/
#ifndef SPACE_COWBOY_OBJLOADER_HPP
#define SPACE_COWBOY_OBJLOADER_HPP

#include <vector>
#include <glm/glm.hpp>
#include <stdio.h>
#include <string>
#include <cstring>


bool loadOBJ(
	const char * path, 
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec3> & out_normals,
	std::vector<glm::vec2> & out_uvs
);
#endif
