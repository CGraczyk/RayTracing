#pragma once

#include "config.hpp"

// Grayscale gradient logic
void grayscale(Canvas &canvas) {
  const int width = canvas.width();
  const int height = canvas.height();

  for (int x = 0; x < width; ++x) {
    double value = static_cast<double>(x) / (width - 1);
    Vec3<double> v = {value, value, value};

    for (int y = 0; y < height; ++y) {
      canvas.set_pixel(x, y, v);
    }
  }
}
