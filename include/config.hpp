#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <numbers>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

// C++ std:: usings

using std::make_shared;
using std::shared_ptr;

// Constants
constexpr int SCREEN_WIDTH = 400;
constexpr int SCREEN_HEIGHT = 400;
constexpr float ASPECT_RATIO = static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT;
constexpr float VIEWPORT_WIDTH = 1.0;
constexpr float VIEWPORT_HEIGHT = VIEWPORT_WIDTH / ASPECT_RATIO;
constexpr float FOCAL_LENGTH = 1.0;

// Tracer Options
constexpr int N_REFLECTIONS = 3;  // surface bounces per ray
constexpr int N_ANTIALIASING = 2; // squared number of rays per pixel

// Utility
const auto pi = std::numbers::pi;

inline auto degrees_to_radians(auto degrees) { return degrees * pi / 180.0; }

// Common Headers

#include "ray.hpp"
#include "vec3.hpp"

#include "canvas.hpp"
#include "light.hpp"