#include <raytracing.hpp>
#include <iostream>
#include <fstream>
#include <vector.hpp>

void write_header(std::ostream& os, uint16_t width, uint16_t height) {
  os << "P3" << std::endl << width << " " << height << std::endl <<  "255" << std::endl;  
}

void demo_draw(uint16_t width, uint16_t height, const std::string& filepath) {
  const auto CONV = 255.99f;
  std::ofstream os{filepath};
  const auto b = float{0.2f};
  write_header(os, width, height);
  
  for (auto i = 0U ; i < height ; ++i) {
    for (auto j = 0U; j < width; ++j) {
      auto v = rt::Vector3f{
        static_cast<float>(i) / height,
        static_cast<float>(j) / width,
        b
      };
      os  << static_cast<int>(v.x() * CONV) << " "
          << static_cast<int>(v.y() * CONV) << " "
          << static_cast<int>(v.z() * CONV) << " ";
    }
    os << std::endl;
  }
}

int main(int argc, char** argv) {
  std::cout << "Hello Wordl" << std::endl;
  demo_draw(800, 600, "image.ppm");
}
