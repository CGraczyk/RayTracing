#pragma once

#include "config.hpp"

class Canvas {
public:
  Canvas(int width, int height)
      : width_(width), height_(height), pixels_(width * height) {
    for (int i = 0; i < width * height; ++i) {
      pixels_[i] = color(255, 255, 255); // Initialize all pixels to black
    }
  }

  void set_pixel(int x, int y, const color &pixel_color) {
    if (x < 0 or x >= width_ or y < 0 or y >= height_) {
      return;
    }
    // Store the screen space pixel columnwise.
    pixels_[y * width_ + x] = pixel_color;
  }

  // Get pixel function
  color get_pixel(int x, int y) const { return pixels_[y * width_ + x]; }

  // Rendering pixel by pixel in Screen Space with origin (0,0) at top left
  // corner´moving right and down.
  void render() const {
    std::cout << "P3\n" << width_ << ' ' << height_ << "\n255\n";

    for (int j = 0; j < height_; j++) {
      std::clog << "\rScanlines remaining: " << (height_ - j) << ' '
                << std::flush;
      for (int i = 0; i < width_; i++) {
        const color &pixel_color = pixels_[j * width_ + i];

        // Clamp the color components to be within the 0-255 range
        int r = std::clamp(pixel_color.x(), 0, 255);
        int g = std::clamp(pixel_color.y(), 0, 255);
        int b = std::clamp(pixel_color.z(), 0, 255);

        std::cout << r << ' ' << g << ' ' << b << '\n';
      }
    }

    std::clog << "\rDone.                 \n";
  }

  int width() { return width_; }

  int height() { return height_; }

private:
  int width_, height_;
  std::vector<color> pixels_;
};