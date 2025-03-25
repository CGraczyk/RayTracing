#include "config.hpp"

#include "renderPipeline.hpp"

// Renderer registry: map string to corresponding function
std::unordered_map<std::string, RenderFunction> plots = {
    {"spectrum", spectrum}, {"grayscale", grayscale}, {"rt1", rt1}};

// Renderer
int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <renderer>\n";
    return 1;
  }

  std::string mode = argv[1];

  if (plots.find(mode) != plots.end()) {
    runRenderer(plots[mode]);
  } else {
    std::cerr << "Unknown renderer: " << mode << "\n";
    return 1;
  }

  return 0;
}