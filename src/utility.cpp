/**
 * @file utility.cpp
 *
 * Defines general purpose utility functions in the "utility" namespace.
 */
#include "utility.hpp"

namespace utility {
std::string readFile(const std::string& filePath) {
	std::string   fileContents;
	std::ifstream fileStream(filePath);

	// Read from file.
	if (fileStream.is_open()) {
		std::stringstream buffer;
		buffer << fileStream.rdbuf();
		fileContents = buffer.str();
	}
	else {
		throw std::runtime_error("File " + filePath + " cannot be found.");
	}

	return fileContents;
}
}
