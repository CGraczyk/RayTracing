#pragma once

#include "config.hpp"

// Grayscale gradient logic
void grayscale(Canvas &canvas) {
  const int width = canvas.width();
  const int height = canvas.height();

  for (int x = 0; x < width; ++x) {
    int value = static_cast<int>(255 * (static_cast<float>(x) / (width - 1)));
    color v = {value, value, value};

    for (int y = 0; y < height; ++y) {
      canvas.set_pixel(x, y, v);
    }
  }
}
