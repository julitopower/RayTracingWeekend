#include <fstream>
#include <iostream>

#include <ray.hpp>
#include <raytracing.hpp>
#include <vector.hpp>

void write_header(std::ostream& os, uint16_t width, uint16_t height) {
  os << "P3" << std::endl << width << " " << height << std::endl <<  "255" << std::endl;  
}

void demo_draw_gradient(uint16_t width, uint16_t height, const std::string& filepath) {
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

void demo_draw_background(uint16_t width, uint16_t height, const std::string& filepath) {
  const auto CONV = 255.99f;
  std::ofstream os{filepath};
  write_header(os, width, height);

  const rt::Vector3f origin{0.0, 0.0, 0.0};
  const rt::Vector3f horizontal_span{4.0, 0.0, 0.0};
  const rt::Vector3f vertical_span{0.0, 2.0, 0.0};
  const rt::Vector3f lower_leftt_corner{-2.0, -1.0, -1.0};

  for (auto i = height - 1 ; i >= 0 ; --i) {
    for (auto j = 0U; j < width; ++j) {
      auto u = static_cast<float>(j) / width;      
      auto v = static_cast<float>(i) / height;

      rt::Ray r{origin, lower_leftt_corner + u * horizontal_span + v * vertical_span};
      rt::Vector3f color = rt::free_ray_color(r);
      os  << static_cast<int>(color.r() * CONV) << " "
          << static_cast<int>(color.g() * CONV) << " "
          << static_cast<int>(color.b() * CONV) << " ";      
    }
    os << std::endl;
  }
}

int main(int argc, char** argv) {
  std::cout << "Hello Wordl" << std::endl;
  demo_draw_gradient(800, 400, "image.ppm");
  demo_draw_background(800, 400, "image2.ppm");
}
