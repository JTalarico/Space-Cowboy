/**
 * @file planetGenerator.hpp
 *
 * Procedural Generation of planets with moons
 */
#ifndef SPACE_COWBOY_PLANET_GENERATOR_HPP
#define SPACE_COWBOY_PLANET_GENERATOR_HPP

#include "moon.hpp"
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
constexpr float MEAN_NUMBER_OF_PLANETS = 10.0f;
constexpr float SDEV_NUMBER_OF_PLANETS = 5.0f;

constexpr float MEAN_ROCKY_PLANET_SIZE = 200.0f;
constexpr float SDEV_ROCKY_PLANET_SIZE = 80.0f;

constexpr float MEAN_GASEOUS_PLANET_SIZE = 500.0f;
constexpr float SDEV_GASEOUS_PLANET_SIZE = 100.0f;

constexpr float MEAN_PLANET_DISTANCE    = 8.0f * MEAN_GASEOUS_PLANET_SIZE;
constexpr float SDEV_PLANET_DISTANCE    = 2.0f * MEAN_GASEOUS_PLANET_SIZE;
constexpr float MIN_PLANET_ORBIT_RADIUS = 2.0f * MEAN_GASEOUS_PLANET_SIZE;

constexpr float MEAN_ANGULAR_VELOCITY = 0.2f;
constexpr float SDEV_ANGULAR_VELOCITY = 0.1f;

constexpr float NUM_MOONS_AVERAGE_PLANET = 2.0f;

constexpr float MIN_MOON_SIZE_FACTOR = 0.05f;
constexpr float MAX_MOON_SIZE_FACTOR = 0.2f;

constexpr float MIN_MOON_DISTANCE_FACTOR = 2.0f;
constexpr float MAX_MOON_DISTANCE_FACTOR = 4.0f;

/**
 * Procedurally generates a vector of planets.
 * Number of planets, Size, Angular velocity and Number of moons for each planet
 * are based on gaussian distributions.
 * Orbit Radius is based on exponential distribution.
 * Orbit angular velocity is based on Orbit Radius (through Kepler's third law)
 * Textures are procedurally generated
 *
 * @return Procedurally generated planets.
 */
std::vector<Planet> generatePlanets();

/**
 * Procedurally generates moons for each of the planets.
 *
 * @param planets Planets to generate moons for.
 * @return Procedurally generated moons.
 */
std::vector<Moon> generateMoons(std::vector<Planet>& planets);


#endif
