/**
* @file diamondsquare.cpp
*
* Implementation file for the diamondsquare class.
**/

#include "diamondsquare.hpp"
/*
std::vector<std::vector<float>>diamondSquare(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, float range, unsigned int level)
{
	if (level < 1) return;//stop function if level reaches 0, MODIFY TO RETURN A VECTOR OF FLOAT VECTORS

	//initialize random seed:
	srand(time(NULL));
	//int randNum = rand() % 1 + 1;

	// diamonds
	for (unsigned i = x1 + level; i < x2; i += level)
		for (unsigned j = y1 + level; j < y2; j += level) {
			float a = m_heightmap[i - level][j - level];
			float b = m_heightmap[i][j - level];
			float c = m_heightmap[i - level][j];
			float d = m_heightmap[i][j];
			float e = m_heightmap[i - level / 2][j - level / 2] = (a + b + c + d) / 4 + rand() * range;
		}

	// squares
	for (unsigned i = x1 + 2 * level; i < x2; i += level)
		for (unsigned j = y1 + 2 * level; j < y2; j += level) {
			float a = m_heightmap[i - level][j - level];
			float b = m_heightmap[i][j - level];
			float c = m_heightmap[i - level][j];
			float d = m_heightmap[i][j];
			float e = m_heightmap[i - level / 2][j - level / 2];

			float f = m_heightmap[i - level][j - level / 2] = (a + c + e + m_heightmap[i - 3 * level / 2][j - level / 2]) / 4 + GetRnd() * range;
			float g = m_heightmap[i - level / 2][j - level] = (a + b + e + m_heightmap[i - level / 2][j - 3 * level / 2]) / 4 + GetRnd() * range;
		}

	diamondSquare(x1, y1, x2, y2, range / 2, level / 2);

}*/
