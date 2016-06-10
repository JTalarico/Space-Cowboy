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
	uvs.reserve(2 * nLatitude * nLongitude);

	// deltaTheta is the angle between lines of latitude. deltaPhi is the angle between lines of
	// longitude.
	const double deltaTheta = M_PI / (nLatitude - 1);
	const double deltaPhi = 2 * M_PI / (nLongitude);

	heightMap.resize(nLatitude, std::vector<float>(nLongitude));/*********************figure out resizing**********************/

	//call diamond square and fill heightmap
	diamondSquare(heightMap, 0, 0, (nLongitude) / 2, 0, smoothness);
	polarize(heightMap);

	unsigned int longitude = 0;

	// Fill the vertices and normals.
	for (unsigned int latitude = 0; latitude < nLatitude; ++latitude) {
		const float n_y = static_cast<float>(cos(latitude * deltaTheta));
		const float y = (radius + heightMap[latitude][0]) * n_y;

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

			//

			//create UVS here, why normals pushed back n_x vs x for vertices, which to push here?
			uvs.push_back(static_cast <float> (rand()) / static_cast <float>(RAND_MAX));
			uvs.push_back(static_cast <float> (rand()) / static_cast <float>(RAND_MAX));
			
			if (longitude == 256)
				break;
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

	//Using the indices we need to readjust the normals
	for (int i = 0; i < indices.size(); i += 3) {

		//Find the corresponding vertices that make up a triangle using Indice data
		float x1 = vertices[3 * indices[i]];
		float y1 = vertices[3 * indices[i] + 1];
		float z1 = vertices[3 * indices[i] + 2];

		float x2 = vertices[3 * indices[i + 1]];
		float y2 = vertices[3 * indices[i + 1] + 1];
		float z2 = vertices[3 * indices[i + 1] + 2];

		float x3 = vertices[3 * indices[i + 2]];
		float y3 = vertices[3 * indices[i + 2] + 1];
		float z3 = vertices[3 * indices[i + 2] + 2];

		//Create two vectors and cross Multiply to find normal
		glm::vec3 vec1 = glm::vec3(x3, y3, z3) - glm::vec3(x1, y1, z1);
		glm::vec3 vec2 = glm::vec3(x2, y2, z2) - glm::vec3(x1, y1, z1);


		glm::vec3 cross = glm::cross(vec1, vec2);
		cross = glm::normalize(cross);

		//Remap the normals to their new data
		normals[3 * indices[i]] = cross.x;
		normals[3 * indices[i] + 1] = cross.y;
		normals[3 * indices[i] + 2] = cross.z;

		normals[3 * indices[i + 1]] = cross.x;
		normals[3 * indices[i + 1] + 1] = cross.y;
		normals[3 * indices[i + 1] + 2] = cross.z;

		normals[3 * indices[i + 2]] = cross.x;
		normals[3 * indices[i + 2] + 1] = cross.y;
		normals[3 * indices[i + 2] + 2] = cross.z;

		//While we have each individual triangle we shall assign the UV data as well

		glm::vec3 p1 = glm::vec3(x1, y1, z1);
		glm::vec3 p2 = glm::vec3(x2, y2, z2);
		glm::vec3 p3 = glm::vec3(x3, y3, z3);

		//This is the only one missing that we haven't previously calculated
		glm::vec3 vec3 = p3 - p2;

		float v1_magnitude = glm::length(vec1);
		float v2_magnitude = glm::length(vec2);
		float v3_magnitude = glm::length(vec3);

		float lengthMax = 0;

		if (v1_magnitude > lengthMax)
			lengthMax = v1_magnitude;

		if (v2_magnitude > lengthMax)
			lengthMax = v2_magnitude;

		if (v3_magnitude > lengthMax)
			lengthMax = v3_magnitude;

		p1 = p1 / lengthMax;
		p2 = p2 / lengthMax;
		p3 = p3 / lengthMax;

		uvs.resize(2 * vertices.size() / 3);

		if (glm::length(p2 - p1) == 1) {

			uvs[2 * indices[i]] = 0.0f;
			uvs[2 * indices[i] + 1] = 0.0f;

			uvs[2 * indices[i + 1]] = 1.0f;
			uvs[2 * indices[i + 1] + 1] = 0.0f;

			float theta = glm::acos(glm::dot((p2 - p1), (p3 - p1)) / (glm::length((p2 - p1))*glm::length((p3 - p1))));

			uvs[2 * indices[i + 2]] = glm::length(p3 - p1) * glm::cos(theta);
			uvs[2 * indices[i + 2] + 1] = glm::length(p3 - p1) * glm::sin(theta);

		}
		if (glm::length(p3 - p1) == 1) {

			uvs[2 * indices[i]] = 0.0f;
			uvs[2 * indices[i] + 1] = 0.0f;

			uvs[2 * indices[i + 2]] = 1.0f;
			uvs[2 * indices[i + 2] + 1] = 0.0f;

			float theta = glm::acos(glm::dot((p3 - p1), (p2 - p1)) / (glm::length((p3 - p1))*glm::length((p2 - p1))));

			uvs[2 * indices[i + 1]] = glm::length(p2 - p1) * glm::cos(theta);
			uvs[2 * indices[i + 1] + 1] = glm::length(p2 - p1) * glm::sin(theta);

		}if (glm::length(p3 - p2) == 1) {

			uvs[2 * indices[i + 1]] = 0.0f;
			uvs[2 * indices[i + 1] + 1] = 0.0f;

			uvs[2 * indices[i + 2]] = 1.0f;
			uvs[2 * indices[i + 2] + 1] = 0.0f;

			float theta = glm::acos(glm::dot((p3 - p2), (p1 - p2)) / (glm::length((p3 - p2))*glm::length((p1 - p2))));

			uvs[2 * indices[i]] = glm::length(p1 - p2) * glm::cos(theta);
			uvs[2 * indices[i] + 1] = glm::length(p1 - p2) * glm::sin(theta);

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
	float r = static_cast <float> (rand()) / static_cast <float>(RAND_MAX);
	float randNum = static_cast <float> ((1.0 * r - 0.5) /pow(smoothness,iteration)/10);
	//std::cout << randNum << std::endl;
	return randNum;
}


//NOTE arraylength must be square 2D with each side length of 2^n+1
void Sphere::diamondSquare(std::vector<std::vector<float>> &heightMap, int x, int y, int stride, int iteration, float smoothness)
{

	if (stride > 0)
	{
		float topLeft = heightMap[x][y];
		float topRight = heightMap[x + 2 * stride][y];
		float bottomLeft = heightMap[x][y + 2 * stride];
		float bottomRight = heightMap[x + 2 * stride][y + 2 * stride];

		int midx = x + stride;
		int midy = y + stride;

		//assign midpoint value
		heightMap[midx][midy] = static_cast <float>((topLeft + topRight + bottomLeft + bottomRight) / 4 + getRand(smoothness, iteration));


		if (!(midx + stride == heightMap.size() - 1 || midx - stride == 0)) {
			heightMap[midx + stride][midy] = (topRight + bottomRight) / 2 + getRand(smoothness + 1.0f, iteration);
			heightMap[midx - stride][midy] = (topLeft + bottomLeft) / 2 + getRand(smoothness + 1.0f, iteration);
		}

		if (!(midy + stride == heightMap.size() - 1 || midy - stride == 0)) {
			heightMap[midx][midy + stride] = (bottomLeft + bottomRight) / 2 + getRand(smoothness + 1.0f, iteration);
			heightMap[midx][midy - stride] = (topLeft + topRight) / 2 + getRand(smoothness + 1.0f, iteration);
		}


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
	

	for (int i = 0; i < heightMap.size(); i++) {

		heightMap[0][i] = 0.0f;
		heightMap[heightMap.size() - 1][i] = 0.0f;

	}

}

void Sphere::textureSphere(const char* textSrc, GLuint &sphere_texture)
{
	glActiveTexture(GL_TEXTURE0); //select texture unit 0

	//GLuint sphere_texture;
	glGenTextures(1, &sphere_texture);
	glBindTexture(GL_TEXTURE_2D, sphere_texture); //bind this texture to the currently bound texture unit

												// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load image, create texture and generate mipmaps
	int sphere_texture_width, sphere_texture_height;
	unsigned char* sphere_image = SOIL_load_image(textSrc, &sphere_texture_width, &sphere_texture_height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sphere_texture_width, sphere_texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, sphere_image);

	SOIL_free_image_data(sphere_image); //free resources
}

