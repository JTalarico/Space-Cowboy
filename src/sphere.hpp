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
//#include "diamondsquare.hpp"
#include <time.h>    
#include <math.h>
#include <iostream>

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
	Sphere(float radius, unsigned int nLatitude, unsigned int nLongitude);//perfect sphere for sun

	//imperfect sphere for planets
	Sphere(float radius, unsigned int nLatitide, unsigned int nLongitude, float smoothness);//imperfect sphere for planets

	// Data members.
	/** Vertices of the sphere. Each triplet of numbers is a sphere vertex. */
	std::vector<GLfloat> vertices;
	/** Sphere normals. Each triplet of numbers is a normal vector of a sphere vertex. */
	std::vector<GLfloat> normals;
	/** Order in which to render the vertices. */
	std::vector<GLuint>  indices;
	/**heightmap used to vary the landscape of the spheres*/
	std::vector<std::vector<float>> heightMap;

	//functions
	/** diamond square algorithm used to created heightmap */
	void diamondSquare(std::vector<std::vector<float>> &heightMap, int x, int y, int stride, int iteration, float smoothness);
	/** returns random num between -1 and 1*/
	float getRand(float smoothness, int iteration);
	/**making sure north and south poles of sphere same value*/
	void polarize(std::vector<std::vector<float>> &heightMap);



};

#endif
