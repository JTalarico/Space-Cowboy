/**
 * @file main.hpp
 *
 * Main header file. Includes header files necessary for the program's main function, along with
 * useful program constants.
 */
#include "camera.hpp"
#include "glfw_guard.hpp"
#include "moon.hpp"
#include "palette.hpp"
#include "planet.hpp"
#include "program.hpp"
#include "stars.hpp"
#include "sun.hpp"
#include "window.hpp"
#include "planetGenerator.hpp"
#include "spacecowboy.hpp"
#include "spaceship.hpp"

#include <exception>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Window properties.
/** Window width in pixels. */
constexpr int  WINDOW_WIDTH   = 2600;
/** Window height in pixels. */
constexpr int  WINDOW_HEIGHT  = 1600;
/** Window title. */
constexpr char WINDOW_TITLE[] = "Space Cowboy";

/** Window background colour. */
constexpr palette::rgb_t WINDOW_BG_COLOUR = palette::BLACK;

// Camera properties.
/** Field of view in radians. */
constexpr float FIELD_OF_VIEW       = 1.2f;
/** Near clipping plane is the distance to the front side of the viewing frustum. */
constexpr float NEAR_CLIPPING_PLANE = 0.1f;
/** Far clipping plane is the distance to the far side of the viewing frustum. */
constexpr float FAR_CLIPPING_PLANE  = 1000000.0f;

/** Point camera is initially looking at. */
const glm::vec3 INITIAL_CAMERA_FRONT    = { 0, 0, -5.0f };
/** Initial position of camera. */
const glm::vec3 INITIAL_CAMERA_POSITION = { 0, 0, 200.0f };
/** Upwards direction of camera view. */
const glm::vec3 INITIAL_UP              = { 0, 1.0f, 0 };

// Sun properties.
/** Size of Sun. */
constexpr float SUN_SIZE = 100.5f;

// Planet properties.
/** Size of planet. */
constexpr float PLANET_SIZE  = 0.5f;
/** Orbital radius. */
constexpr float ORBIT_RADIUS = 2.5f;

/** Planet's angular velocity. */
const glm::vec3 PLANET_ANGULAR_VELOCITY         = { 0, 0.0f, 0 };
/** Planet's orbital angular velocity. */
const glm::vec3 PLANET_ORBITAL_ANGULAR_VELOCITY = { 0, 0.0f, 0 };
