/**
 * @file sphere.cpp
 *
 * Implementation file for the Sphere class.
 */
#include "sphere.hpp"

// Constructors.
Sphere::Sphere(float radius, unsigned int nLatitude, unsigned int nLongitude) {
	// Assertions to check that input parameters make sense.
	assert(radius > 0);
	assert(nLatitude > 3);
	assert(nLongitude > 3);

	// Set the capacity of vertices so multiple resizes are not necessary.
	vertices.reserve(3 * nLatitude * nLongitude);
	indices.reserve(6 * nLatitude * nLongitude);

	// deltaTheta is the angle between lines of latitude. deltaPhi is the angle between lines of
	// longitude.
	const double deltaTheta = M_PI / (nLatitude - 1);
	const double deltaPhi   = 2 * M_PI / (nLongitude);

	// Fill the vertices.
	for (unsigned int latitude = 0; latitude < nLatitude; ++latitude) {
		const float y = radius * static_cast<float>(cos(latitude * deltaTheta));

		for (unsigned int longitude = 0; longitude < nLongitude; ++longitude) {
			const float x = radius * static_cast<float>(sin(longitude * deltaPhi)) *
			                static_cast<float>(sin(latitude * deltaTheta));
			const float z = radius * static_cast<float>(cos(longitude * deltaPhi)) *
			                static_cast<float>(sin(latitude * deltaTheta));

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}

	// Fill the indices.
	// Break the sphere into groups of squares. Each square is then broken down into two triangles.
	for (unsigned int latitude = 0; latitude < nLatitude - 1; ++latitude) {
		for (unsigned int longitude = 0; longitude < nLongitude; ++longitude) {
			indices.push_back(latitude * nLongitude + longitude);
			indices.push_back(latitude * nLongitude + (longitude + 1) % nLongitude);
			indices.push_back((latitude + 1) * nLongitude + (longitude + 1) % nLongitude);
			indices.push_back((latitude + 1) * nLongitude + (longitude + 1) % nLongitude);
			indices.push_back((latitude + 1) * nLongitude + longitude);
			indices.push_back(latitude * nLongitude + longitude);
		}
	}
}
