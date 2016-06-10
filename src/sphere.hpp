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
#include <time.h>    
#include <math.h>
#include <iostream>
#include <string>
#include "glm\glm.hpp"

#include "soil\SOIL.h"

/**
 * Contains the mesh data required to render a sphere.
 */
class Sphere {
public:
	// Constructors.
	/**
	 * Constructs a perfect sphere with the given parameters.
	 *
	 * @param radius Radius of the sphere.
	 * @param nLatitude Lines of latitude.
	 * @param nLongitude Lines of longitude.
	 */
	Sphere(float radius, unsigned int nLatitude, unsigned int nLongitude);//perfect sphere for sun

	/**
	* Constructs an imperfect sphere with the given parameters.
	*
	* @param radius Radius of the sphere.
	* @param nLatitude Lines of latitude.
	* @param nLongitude Lines of longitude.
	* @param smoothness Smoothness of the terrain.
	*/
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
	/**UV coordinates of the sphere. Each pair of numbers correspond to the texture coordinates of a sphere vertex*/
	std::vector<GLfloat> uvs;

	//functions
	/** diamond square algorithm used to create heightmap */
	void diamondSquare(std::vector<std::vector<float>> &heightMap, int x, int y, int stride, int iteration, float smoothness);
	/** returns random num between -0.1 and 0.1*/
	float getRand(float smoothness, int iteration);
	/**making sure the north and south poles of sphere are the same value*/
	void polarize(std::vector<std::vector<float>> &heightMap);
	/**apply texture to sphere based on generated heightmap*/
	void textureSphere(const char* textSrc, GLuint &sphere_texture);



};

#endif
