/**
 * @file sphere.cpp
 *
 * Implementation file for the Sphere class.
 */
#include "sphere.hpp"

 // Constructors.
//constructor for imperfect sphere
Sphere::Sphere(float radius, unsigned int nLatitude, unsigned int nLongitude, float smoothness) {

	// Assertions to check that input parameters make sense.
	assert(radius > 0);
	assert(nLatitude > 2);
	assert(nLongitude > 3);

	// Set the capacity of vertices, normals, and indices so multiple resizes are not necessary.
	vertices.reserve(3 * nLatitude * nLongitude);
	normals.reserve(3 * nLatitude * nLongitude);
	indices.reserve(6 * nLatitude * nLongitude);

	// deltaTheta is the angle between lines of latitude. deltaPhi is the angle between lines of
	// longitude.
	const double deltaTheta = M_PI / (nLatitude - 1);
	const double deltaPhi = 2 * M_PI / (nLongitude);

	heightMap.resize(257, std::vector<float>(257));

	//call diamond square and fill heightmap
	diamondSquare(heightMap, 0, 0, 240 / 2, 0, smoothness);
	polarize(heightMap);

	unsigned int longitude = 0;

	// Fill the vertices and normals.
	for (unsigned int latitude = 0; latitude < nLatitude; ++latitude) {
		const float n_y = static_cast<float>(cos(latitude * deltaTheta));
		const float y = (radius + heightMap[latitude][longitude]) * n_y;

		for (longitude = 0; longitude < nLongitude; ++longitude) {
			const float n_x = static_cast<float>(sin(longitude * deltaPhi)) *
				static_cast<float>(sin(latitude * deltaTheta));
			const float n_z = static_cast<float>(cos(longitude * deltaPhi)) *
				static_cast<float>(sin(latitude * deltaTheta));

			const float x = (radius + heightMap[latitude][longitude]) * n_x;
			const float z = (radius + heightMap[latitude][longitude]) * n_z;

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			normals.push_back(n_x);
			normals.push_back(n_y);
			normals.push_back(n_z);
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

// Constructors.
Sphere::Sphere(float radius, unsigned int nLatitude, unsigned int nLongitude) {

	

	// Assertions to check that input parameters make sense.
	assert(radius > 0);
	assert(nLatitude > 2);
	assert(nLongitude > 3);

	// Set the capacity of vertices, normals, and indices so multiple resizes are not necessary.
	vertices.reserve(3 * nLatitude * nLongitude);
	normals.reserve(3 * nLatitude * nLongitude);
	indices.reserve(6 * nLatitude * nLongitude);

	// deltaTheta is the angle between lines of latitude. deltaPhi is the angle between lines of
	// longitude.
	const double deltaTheta = M_PI / (nLatitude - 1);
	const double deltaPhi   = 2 * M_PI / (nLongitude);


	// Fill the vertices and normals.
	for (unsigned int latitude = 0; latitude < nLatitude; ++latitude) {
		const float n_y = static_cast<float>(cos(latitude * deltaTheta));
		const float y   = radius  * n_y ;

		for (unsigned int longitude = 0; longitude < nLongitude; ++longitude) {
			const float n_x = static_cast<float>(sin(longitude * deltaPhi)) *
			                  static_cast<float>(sin(latitude * deltaTheta));
			const float n_z = static_cast<float>(cos(longitude * deltaPhi)) *
			                  static_cast<float>(sin(latitude * deltaTheta));

			const float x = radius * n_x;
			const float z = radius * n_z;

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			normals.push_back(n_x);
			normals.push_back(n_y);
			normals.push_back(n_z);
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
float Sphere::getRand(float smoothness, int iteration)
{
	float r = static_cast <float> (rand() - 1) / static_cast <float>(RAND_MAX);
	float randNum = static_cast <float> ((2 * r - 1) /pow(smoothness,iteration)/10);
	//std::cout << randNum << std::endl;
	return randNum;
}


//NOTE arraylength must be square 2D with each side length of 2^n+1
void Sphere::diamondSquare(std::vector<std::vector<float>> &heightMap, int x, int y, int stride, int iteration, float smoothness)
{

	if (stride > 0)
	{
		float topLeft = heightMap[x][y];
		float topRight = heightMap[x + 2*stride][y];
		float bottomLeft = heightMap[x][y + 2 * stride];
		float bottomRight = heightMap[x + 2 * stride][y + 2 * stride];

		int midx = x + stride;
		int midy = y + stride;

		//assign midpoint value
		heightMap[midx][midy] = static_cast <float>((topLeft + topRight + bottomLeft + bottomRight) / 4 + getRand(smoothness, iteration));

		heightMap[midx + stride][midy] = (topRight + bottomRight)/2 + getRand(smoothness, iteration);
		heightMap[midx - stride][midy] = (topLeft + bottomLeft)/2 + getRand(smoothness, iteration);
		heightMap[midx][midy + stride] = (bottomRight + bottomRight)/2 + getRand(smoothness, iteration);
		heightMap[midx][midy - stride] = (topLeft + topRight)/2 + getRand(smoothness, iteration);

		iteration++;
		diamondSquare(heightMap, x, y, stride / 2, iteration, smoothness);
		diamondSquare(heightMap, midx, y, stride / 2, iteration, smoothness);
		diamondSquare(heightMap, x, midy, stride / 2, iteration, smoothness);
		diamondSquare(heightMap, midx, midy, stride / 2, iteration, smoothness);

	}

}

//Since we want to make onto a sphere we make the north and south pole mappings all the same value
void Sphere::polarize(std::vector<std::vector<float>> &heightMap)
{
	//make the northpole the same value as southpole

	float northTotal = 0.0f;
	float southTotal = 0.0f;

	for (int i = 0; i < heightMap.size(); i++)
	{
		northTotal += heightMap[i][0];
		southTotal += heightMap[i][heightMap.size() - 1];
	}

	float northAverage = northTotal / heightMap.size();
	float southAverage = southTotal / heightMap.size();

	for (int i = 0; i < heightMap.size(); i++) {

		heightMap[i][0] = northAverage;
		heightMap[i][heightMap.size() - 1] = southAverage;

	}

}

