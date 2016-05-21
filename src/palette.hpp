/**
 * @file palette.hpp
 *
 * Defines a set of default colours and opacities in the "palette" namespace.
 */
#ifndef SPACE_COWBOY_PALETTE_HPP
#define SPACE_COWBOY_PALETTE_HPP

#include <array>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace palette {
/** An array of three floats is interpreted as an RGB value triplet. */
using rgb_t = std::array<GLfloat, 3>;

// Colours.
constexpr rgb_t WHITE   = { 1.0f, 1.0f, 1.0f };
constexpr rgb_t SILVER  = { 0.75f, 0.75f, 0.75f };
constexpr rgb_t GREY    = { 0.5f, 0.5f, 0.5f };
constexpr rgb_t BLACK   = { 0.0f, 0.0f, 0.0f };
constexpr rgb_t RED     = { 1.0f, 0.0f, 0.0f };
constexpr rgb_t MAROON  = { 0.5f, 0.0f, 0.0f };
constexpr rgb_t YELLOW  = { 1.0f, 1.0f, 0.0f };
constexpr rgb_t OLIVE   = { 0.5f, 0.5f, 0.0f };
constexpr rgb_t LIME    = { 0.0f, 1.0f, 0.0f };
constexpr rgb_t GREEN   = { 0.0f, 0.5f, 0.0f };
constexpr rgb_t AQUA    = { 0.0f, 1.0f, 1.0f };
constexpr rgb_t TEAL    = { 0.0f, 0.5f, 0.5f };
constexpr rgb_t BLUE    = { 0.0f, 0.0f, 1.0f };
constexpr rgb_t NAVY    = { 0.0f, 0.0f, 0.5f };
constexpr rgb_t FUCHSIA = { 1.0f, 0.0f, 1.0f };
constexpr rgb_t PURPLE  = { 0.5f, 0.0f, 0.5f };

// Opacities.
constexpr GLfloat OPAQUE      = 1.0f;
constexpr GLfloat TRANSLUCENT = 0.5f;
constexpr GLfloat TRANSPARENT = 0.0f;
}

#endif
