#include "canvas.hpp"

// Function signature
void raytracer(Canvas &canvas);
void spectrum(Canvas &canvas);
void grayscale(Canvas &canvas);

int main() {
  Canvas canvas(SCREEN_WIDTH, SCREEN_HEIGHT);

  // Call the user-provided function to set pixel values
  raytracer(canvas);

  // Render the final result
  canvas.render();
}