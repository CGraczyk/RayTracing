#pragma once

#include "config.hpp"

// Function signature
using RenderFunction = std::function<void(Canvas &)>;

// Unified rendering function
void runRenderer(RenderFunction renderFunc);

// Function declarations for different renderers
void spectrum(Canvas &canvas);
void grayscale(Canvas &canvas);
void rt1(Canvas &canvas);