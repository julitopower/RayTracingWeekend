#include <fstream>
#include <iostream>

#include <hitable.hpp>
#include <hitable_list.hpp>
#include <ray.hpp>
#include <raytracing.hpp>
#include <vector.hpp>
#include <sphere.hpp>

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

void render(uint16_t width, uint16_t height, const rt::Hitable& world, const std::string& filepath) {
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
      rt::Vector3f color = rt::ray_color(r, world);
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
  rt::HitableList::HitablePtr worldVector;
  worldVector.push_back(std::make_unique<rt::Sphere>(rt::Vector3f{0, 0, -1}, 0.5));
  worldVector.push_back(std::make_unique<rt::Sphere>(rt::Vector3f{0, -100.5, -1}, 100));
  auto world = rt::HitableList{std::move(worldVector)};
  render(800, 400, world, "image2.ppm");
}
