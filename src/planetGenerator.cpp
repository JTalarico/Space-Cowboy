#include "planetGenerator.hpp"


std::vector <Planet> generatePlanets() {

	std::random_device rd;
	std::mt19937 e2(rd());

	// Set number of planets
	std::normal_distribution<> planetNumberDistribution(MEAN_NUMBER_OF_PLANETS, SDEV_NUMBER_OF_PLANETS);
	unsigned long numberOfPlanets = static_cast<unsigned long>(planetNumberDistribution(e2));

	std::vector <Planet> planets(numberOfPlanets);

	// Change Size of each planet
	std::normal_distribution<float> planetSizeDistribution(MEAN_PLANET_SIZE, SDEV_PLANET_SIZE);
	for(std::vector<Planet>::iterator planet = planets.begin(); planet != planets.end(); ++planet)
		planet->scale(planetSizeDistribution(e2));

	// Change Color
		// Procedurally generated textures


	// Change orbit
	std::normal_distribution<float> planetOrbitDistribution(MEAN_PLANET_DISTANCE, SDEV_PLANET_DISTANCE);
	std::uniform_real_distribution<float> angle(0, static_cast<float>(2.0 * M_PI));

	float orbitRadius = MIN_PLANET_ORBIT_RADIUS + planetOrbitDistribution(e2);
	float randomAngle = angle(e2);
	planets[0].translate(orbitRadius * glm::cos(randomAngle), 0, orbitRadius * glm::sin(randomAngle));

	for(int i = 1; i < numberOfPlanets; i++) {
		orbitRadius = glm::length(planets[i - 1].position()) + planetOrbitDistribution(e2);
		randomAngle = angle(e2);
		planets[i].translate(orbitRadius * glm::cos(randomAngle), 0, orbitRadius * glm::sin(randomAngle));
	}
	// Change orbital angular velocity
	for (Planet& planet : planets)
		planet.setOrbitalAngularVelocity({ 0, 5.0f * pow(1.0f/glm::length(planet.position()), 2.0f/3.0f), 0 });

	// Change angular velocity
	std::normal_distribution<float> planetAngularVelocity(MEAN_ANGULAR_VELOCITY, SDEV_ANGULAR_VELOCITY);

	for (Planet& planet : planets)
		planet.setAngularVelocity({ 0, planetAngularVelocity(e2), 0 });

	return planets;
}
