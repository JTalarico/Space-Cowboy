/**
 * @file main.cpp
 *
 * Contains the program's main entry point.
 */
#include "main.hpp"

/**
 * Main entry point of the program.
 */
int main() {

	srand(time(NULL));//seed random num generator

	try {
		// Create an OpenGL context by initializing GLFW. Note that this step exploits RAII: the
		// constructor for the GLFWGuard class calls the various GLFW initialization functions,
		// while GLFWGuard's destructor calls glfwTerminate to terminate GLFW. This ensures that
		// GLFW is properly terminated regardless of whether an exception is thrown or not.
		GLFWGuard glfwGuard;

		// Create window and bind to current OpenGL context. Enable z buffer depth testing.
		Window window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
		window.enable();
		window.enableDepthTesting();

		// Initialize GLEW with experimental features enabled.
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			throw std::runtime_error("Failed to start GLEW.");
		}

		// Enable v-sync.
		glfwSwapInterval(1);

		// Set window background colour.
		window.setBgColour(WINDOW_BG_COLOUR, palette::OPAQUE);

		// Create camera. This object sets the viewing properties such as the camera position and
		// field of view.
		Camera camera(FIELD_OF_VIEW, window.aspectRatio(), NEAR_CLIPPING_PLANE, FAR_CLIPPING_PLANE,
		              INITIAL_CAMERA_POSITION, INITIAL_CAMERA_FRONT, INITIAL_UP);

		// Pass camera object to window.
		window.setCamera(&camera);

		Stars stars;

		// Create Sun. Scale.
		Sun sun;
		sun.scale(SUN_SIZE);

		// Create planets using procedural generation.
		std::vector<Planet> rockyPlanets = generatePlanets(Planet::ROCKY, 0);
		std::vector<Planet> gaseousPlanets = generatePlanets(Planet::GASEOUS, glm::length(rockyPlanets.back().position()));

		// Create moons for the planets using procedural generation.
		std::vector<Moon> rockyMoons = generateMoons(rockyPlanets);
		std::vector<Moon> gaseousMoons = generateMoons(gaseousPlanets);

		// Create spaceship and spacecowboy.
		Spaceship   spaceship;
		Spacecowboy spacecowboy;

		//choose random planet on which to spawn spacecowboy
		int randomPlanet = rand() % gaseousPlanets.size();

		#if PLAY_MUSIC
			system("mplayer ./music/interstellar.mp3  </dev/null >/dev/null 2>&1 &");
		#endif

		// Game loop.
		while (not window.shouldClose()) {
			// Set time difference between last and current frame
			window.updateDeltaTime();
			window.updatePosition();

			// Clear the screen and poll for event triggers.
			window.clear();
			window.pollEvents();

			// Adjust camera's aspect ratio and the window's viewport in case window dimensions
			// changed.
			camera.setAspectRatio(window.aspectRatio());
			window.setViewport();


			// Update all Positions and states of Objects in the game

			// Update planet's state.
			for (Planet& planet : rockyPlanets) {
				planet.updateState();
			}
			for (Planet& planet : gaseousPlanets) {
				planet.updateState();
			}

			// Update moons.
			for (Moon& moon : rockyMoons) {
				moon.updateState();
			}
			for (Moon& moon : gaseousMoons) {
				moon.updateState();
			}

			// Update spaceships's and spacecowboy's state.
			spaceship.updateState(camera);
			spacecowboy.updateState(camera, gaseousPlanets[randomPlanet]);

			//Check for collisions with planets
			for (Planet& planet : rockyPlanets) {
				if (planet.planetCollision(camera)) {

					window.setCollisison(true);
					window.setBounce(glm::normalize(glm::vec3(camera.position() - planet.position()) * 2.0f));
					break;
				}
				//window.setCollisison(false);
			}

			for (Planet& planet : gaseousPlanets) {
				if (planet.planetCollision(camera)) {

					window.setCollisison(true);
					window.setBounce(glm::normalize(glm::vec3(camera.position() - planet.position()) * 2.0f));
					break;
				}
				//window.setCollisison(false);
			}

			for (Moon& moon : rockyMoons) {
				if (moon.moonCollision(camera)) {

					window.setCollisison(true);
					window.setBounce(glm::normalize(
						glm::vec3(camera.position() - moon.position()) * 2.0f));
					break;
				}
				//window.setCollisison(false);
			}
			for (Moon& moon : gaseousMoons) {
				if (moon.moonCollision(camera)) {

					window.setCollisison(true);
					window.setBounce(glm::normalize(
							glm::vec3(camera.position() - moon.position()) * 2.0f));
					break;
				}
				//window.setCollisison(false);
			}

			// Test for sun collision
			if (sun.sunCollision(camera)) {
				window.setCollisison(true);
				window.setBounce(glm::normalize(glm::vec3(camera.position()) * 2.0f));
			}

			// Draw stars.
			stars.draw(camera);

			// Sun and Planets created from same sphere algorithm therefore same culling orientation
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CW);

			// Draw the Sun.
			sun.draw(camera);

			// Draw the planets.
			for (const Planet& planet : rockyPlanets) {
				planet.draw(camera);
			}
			for (const Planet& planet : gaseousPlanets) {
				planet.draw(camera);
			}

			// Draw the moons.
			for (const Moon& moon : rockyMoons) {
				moon.draw(camera);
			}
			for (const Moon& moon : gaseousMoons) {
				moon.draw(camera);
			}

			//Assets (Space Ship & DeadPool) loaded with a CCW orientation
			glFrontFace(GL_CCW);

			// Draw the spaceship.
			spaceship.draw(camera);

			// Draw the spacecowboy.
			spacecowboy.draw(camera);

			// Swap the front and back buffers.
			window.swapBuffers();
		}
		#if PLAY_MUSIC
			system("kill -9 $(pgrep -f \"mplayer\")");
		#endif

	}
	catch (const std::exception& e) {
		// Print error message to stderr and exit.
		std::cerr << "ERROR: " << e.what() << std::endl;
		std::cerr << "Exiting.\n";
		return 1;
	}
}
