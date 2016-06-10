/**
 * @file planetGenerator.hpp
 *
 * Procedural Generation of planets with moons
 */
#ifndef SPACE_COWBOY_PLANET_GENERATOR_HPP
#define SPACE_COWBOY_PLANET_GENERATOR_HPP

#include "planet.hpp"

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

// Random Gaussian generation
#include <iomanip>
#include <map>
#include <random>

// Constants
const float MEAN_NUMBER_OF_PLANETS = 15.0f;
const float SDEV_NUMBER_OF_PLANETS = 5.0f;

const float MEAN_PLANET_SIZE = 120.0f;
const float SDEV_PLANET_SIZE  = 50.0f;

const float MEAN_PLANET_DISTANCE = 10.0f * MEAN_PLANET_SIZE;
const float SDEV_PLANET_DISTANCE = 2.0f * MEAN_PLANET_SIZE;
const float MIN_PLANET_ORBIT_RADIUS = 2.0f * MEAN_PLANET_SIZE;

const float MEAN_ANGULAR_VELOCITY = 0.5f;
const float SDEV_ANGULAR_VELOCITY = 0.2f;


/**
 * Procedurally generates a vector of planets.
 * Number of planets, Size, Angular velocity and Number of moons for each planet
 * are based on gaussian distributions.
 * Orbit Radius is based on exponential distribution.
 * Orbit angular velocity is based on Orbit Radius (through Kepler's third law)
 * Textures are procedurally generated
 *
 * @return Procedurally generated planets with moons.
 */
std::vector <Planet> generatePlanets();



#endif