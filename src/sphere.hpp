/**
 * @file sphere.hpp
 *
 * Interface file for the Sphere class.
 */
#ifndef SPACE_COWBOY_SPHERE_HPP
#define SPACE_COWBOY_SPHERE_HPP

#include <cassert>
#include <cmath>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 * Contains the mesh data required to render a sphere.
 */
class Sphere {
public:
	// Constructors.
	/**
	 * Constructs a sphere with the given parameters.
	 *
	 * @param radius Radius of the sphere.
	 * @param nLatitude Lines of latitude.
	 * @param nLongitude Lines of longitude.
	 */
	Sphere(float radius, unsigned int nLatitude, unsigned int nLongitude);

	// Data members.
	/** Vertices of the sphere. */
	std::vector<GLfloat> vertices;
	/** Order in which to render the vertices. */
	std::vector<GLuint>  indices;
};

#endif
