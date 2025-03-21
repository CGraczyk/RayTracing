#pragma once

#include "canvas.hpp"
#include "config.hpp"
#include <functional>

// Function signature
using RenderFunction = std::function<void(Canvas &)>;

// Unified rendering function
void runRenderer(RenderFunction renderFunc);

// Function declarations for different renderers
void spectrum(Canvas &canvas);
void grayscale(Canvas &canvas);
void rt1(Canvas &canvas);