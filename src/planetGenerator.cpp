#include "planetGenerator.hpp"


std::vector<Planet> generatePlanets(Planet::TERRAIN_TYPE planetType, float minDistance) {
	float MEAN_NUMBER_OF_PLANETS;
	float SDEV_NUMBER_OF_PLANETS;
	float MEAN_PLANET_SIZE;
	float SDEV_PLANET_SIZE;
	Planet::TERRAIN_TYPE PLANET_TEXTURE;
	float MEAN_PLANET_DISTANCE;
	float SDEV_PLANET_DISTANCE;
	float MIN_PLANET_ORBIT_RADIUS;

	if (planetType == Planet::ROCKY) {
		MEAN_NUMBER_OF_PLANETS = MEAN_NUMBER_OF_ROCKY_PLANETS;
		SDEV_NUMBER_OF_PLANETS = SDEV_NUMBER_OF_ROCKY_PLANETS;
		MEAN_PLANET_SIZE = MEAN_ROCKY_PLANET_SIZE;
		SDEV_PLANET_SIZE = SDEV_ROCKY_PLANET_SIZE;
		PLANET_TEXTURE = Planet::ROCKY;
		MEAN_PLANET_DISTANCE = MEAN_ROCKY_PLANET_DISTANCE;
		SDEV_PLANET_DISTANCE = SDEV_ROCKY_PLANET_DISTANCE;
		MIN_PLANET_ORBIT_RADIUS = MIN_ROCKY_PLANET_ORBIT_RADIUS;
	}
	else if (planetType == Planet::GASEOUS) {
		MEAN_NUMBER_OF_PLANETS = MEAN_NUMBER_OF_GASEOUS_PLANETS;
		SDEV_NUMBER_OF_PLANETS = SDEV_NUMBER_OF_GASEOUS_PLANETS;
		MEAN_PLANET_SIZE = MEAN_GASEOUS_PLANET_SIZE;
		SDEV_PLANET_SIZE = SDEV_GASEOUS_PLANET_SIZE;
		PLANET_TEXTURE = Planet::GASEOUS;
		MEAN_PLANET_DISTANCE = MEAN_GASEOUS_PLANET_DISTANCE;
		SDEV_PLANET_DISTANCE = SDEV_GASEOUS_PLANET_DISTANCE;
		MIN_PLANET_ORBIT_RADIUS = minDistance;
		Planet::makeRocky = false;

	}
	else {
		MEAN_NUMBER_OF_PLANETS = MEAN_NUMBER_OF_GASEOUS_PLANETS;
		SDEV_NUMBER_OF_PLANETS = SDEV_NUMBER_OF_GASEOUS_PLANETS;
		MEAN_PLANET_SIZE = MEAN_GASEOUS_PLANET_SIZE;
		SDEV_PLANET_SIZE = SDEV_GASEOUS_PLANET_SIZE;
		PLANET_TEXTURE = Planet::GASEOUS;
		MEAN_PLANET_DISTANCE = MEAN_GASEOUS_PLANET_DISTANCE;
		SDEV_PLANET_DISTANCE = SDEV_GASEOUS_PLANET_DISTANCE;
		MIN_PLANET_ORBIT_RADIUS = minDistance;
		Planet::makeRocky = false;
	}

	std::random_device rd;
	std::mt19937       e2(rd());

	// Set number of planets
	std::normal_distribution<> planetNumberDistribution(MEAN_NUMBER_OF_PLANETS,
	                                                    SDEV_NUMBER_OF_PLANETS);
	unsigned long              numberOfPlanets = static_cast<unsigned long>(planetNumberDistribution(
			e2));

	std::vector<Planet> planets(numberOfPlanets);

	// Change Size of each planet
	std::normal_distribution<float>    planetSizeDistribution(MEAN_PLANET_SIZE, SDEV_PLANET_SIZE);
	for (std::vector<Planet>::iterator planet  = planets.begin();
	     planet != planets.end(); ++planet) {
		planet->scale(planetSizeDistribution(e2));
	}

	// Change Color
	for (Planet& planet : planets) {
		planet.setPlanetTextureType(PLANET_TEXTURE);
	}

	// Change orbit
	std::normal_distribution<float>       planetOrbitDistribution(MEAN_PLANET_DISTANCE,
	                                                              SDEV_PLANET_DISTANCE);
	std::uniform_real_distribution<float> angle(0, static_cast<float>(2.0 * M_PI));

	float orbitRadius = MIN_PLANET_ORBIT_RADIUS + planetOrbitDistribution(e2);
	float randomAngle = angle(e2);
	planets[0].translate(orbitRadius * glm::cos(randomAngle), 0,
	                     orbitRadius * glm::sin(randomAngle));

	for (int i = 1; i < numberOfPlanets; i++) {
		orbitRadius = glm::length(planets[i - 1].position()) + planetOrbitDistribution(e2);
		randomAngle = angle(e2);
		planets[i].translate(orbitRadius * glm::cos(randomAngle), 0,
		                     orbitRadius * glm::sin(randomAngle));
	}
	// Change orbital angular velocity
	for (Planet& planet : planets) {
		planet.setOrbitalAngularVelocity(
				{ 0, 2.0f * pow(1.0f / glm::length(planet.position()), 2.0f / 3.0f), 0 });
	}

	// Change angular velocity
	std::normal_distribution<float> planetAngularVelocity(MEAN_ANGULAR_VELOCITY,
	                                                      SDEV_ANGULAR_VELOCITY);

	for (Planet& planet : planets) {
		planet.setAngularVelocity({ 0, planetAngularVelocity(e2), 0 });
	}
	Planet::makeRocky = true;

	return planets;
}

std::vector<Moon> generateMoons(std::vector<Planet>& planets) {
	// Create random number engine.
	std::random_device rd;
	std::mt19937       rng(rd());

	// Get number of moons for each planet.
	std::vector<int>  nMoonsPerPlanet(planets.size());
	for (unsigned int i = 0; i < nMoonsPerPlanet.size(); ++i) {
		nMoonsPerPlanet[i] = static_cast<int>(NUM_MOONS_AVERAGE_PLANET * planets[i].size() /
		                                      MEAN_ROCKY_PLANET_SIZE);
	}

	// Get total number of moons.
	unsigned int nMoonsTotal = 0;
	for (int     nMoons : nMoonsPerPlanet) {
		nMoonsTotal += nMoons;
	}

	std::vector<Moon> moons(nMoonsTotal);

	// Set properties for each moon.
	unsigned int      i = 0;
	for (unsigned int j = 0; j < planets.size(); ++j) {
		Planet& planet = planets[j];
		int nMoons = nMoonsPerPlanet[j];

		for (unsigned int k = 0; k < nMoons; ++k) {
			Moon& moon = moons[i + k];

			// Set moon's texture.
			moon.setMoonTexture();

			// Set planet.
			moon.setPrimary(&planet);

			// Set moon size.
			float scaleFactor = std::uniform_real_distribution<float>(MIN_MOON_SIZE_FACTOR,
			                                                          MAX_MOON_SIZE_FACTOR)(rng);
			moon.scale(scaleFactor * planet.size());

			// Position the moon.
			float angle       = std::uniform_real_distribution<float>(
					0.0f, 2 * static_cast<float>(M_PI))(rng);
			float orbitRadius = std::uniform_real_distribution<float>(
					MIN_MOON_DISTANCE_FACTOR * planet.size(),
					MAX_MOON_DISTANCE_FACTOR * planet.size())(rng);
			moon.translate(planet.position() +
			               glm::vec3(orbitRadius * glm::cos(angle), 0.0f,
			                         orbitRadius * glm::sin(angle)));

			// Set orbital angular velocity.
			float orbitalAngularSpeed =
					      15.0f * static_cast<float>(pow(1.0f / orbitRadius, 2.0f / 3.0f));
			moon.setOrbitalAngularVelocity(0.0f, orbitalAngularSpeed, 0.0f);

			// Set angular velocity.
			float angularSpeed = std::uniform_real_distribution<float>(-orbitalAngularSpeed,
			                                                           orbitalAngularSpeed)(rng);
			moon.setAngularVelocity(0.0f, angularSpeed, 0.0f);
		}

		i += nMoons;
	}

	return moons;
}
