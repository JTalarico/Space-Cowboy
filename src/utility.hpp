/**
 * @file utility.hpp
 *
 * Declares general purpose utility functions in the "utility" namespace.
 */
#ifndef SPACE_COWBOY_UTILITY_HPP
#define SPACE_COWBOY_UTILITY_HPP

#include <fstream>
#include <sstream>
#include <string>

namespace utility {
/**
 * Returns the contents of specified file.
 *
 * @param filePath Path to file.
 * @return Contents of file.
 * @throws std::runtime_exception if file cannot be read.
 */
std::string readFile(const std::string& filePath);
}

#endif
