#include <raytracing.hpp>
#include <iostream>
#include <fstream>

void demo_draw(uint16_t width, uint16_t height, const std::string& filepath) {
  const auto CONV = 255.99f;
  std::ofstream os{filepath};
  auto b = int(0.2f  * CONV);
  os << "P3" << std::endl << width << " " << height << std::endl <<  "255" << std::endl;
  
  for (auto i = 0U ; i < height ; ++i) {
    for (auto j = 0U; j < width; ++j) {
      auto r = static_cast<int>(CONV * (i / static_cast<float>(height)));
      auto g = static_cast<int>(CONV * (j / static_cast<float>(width)));
      os << r << " " << g << " " << b << " ";
    }
    os << std::endl;
  }
}

int main(int argc, char** argv) {
  std::cout << "Hello Wordl" << std::endl;
  demo_draw(800, 600, "image.ppm");
}
