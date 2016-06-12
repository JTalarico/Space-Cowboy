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

		// Create planets useing procedural generation.
		std::vector <Planet> planets = generatePlanets();

		
		Spaceship spaceship;

		Spacecowboy spacecowboy;

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
			for(Planet& planet : planets)
				planet.updateState();

			// Update spaceships's and spacecowboy's state.
			spaceship.updateState(camera);
			spacecowboy.updateState(camera);

			//Check for collisions with planets
			for (Planet& planet : planets) {
				if (planet.planetCollision(camera)) {

					window.setCollisison(true);
					window.setBounce(glm::normalize(glm::vec3(camera.position() - planet.position())*2.0f));
					break;
				}
				window.setCollisison(false);
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
			for(const Planet& planet : planets)
				planet.draw(camera);
			
			//Assets (Space Ship & DeadPool) loaded with a CCW orientation
			glFrontFace(GL_CCW);

			// Draw the spaceship.
			spaceship.draw(camera);
			
			// Draw the spacecowboy.
			spacecowboy.draw(camera);
	
		
			// Swap the front and back buffers.
			window.swapBuffers();
		}

		return 0;
	}
	catch (const std::exception& e) {
		// Print error message to stderr and exit.
		std::cerr << "ERROR: " << e.what() << std::endl;
		std::cerr << "Exiting.\n";
		return 1;
	}
}
