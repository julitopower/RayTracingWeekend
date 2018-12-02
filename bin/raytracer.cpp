#include <fstream>
#include <iostream>
#include <random>

#include <camera.hpp>
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

void render(uint16_t width,
            uint16_t height,
            const rt::Hitable& world,
            const rt::Camera& cam,
            uint anti_alias,
            const std::string& filepath) {
  const auto CONV = 255.99f;
  std::ofstream os{filepath};
  write_header(os, width, height);

  auto dis = std::uniform_real_distribution<>{0.0, 1.0};
  std::random_device device;
  
  for (auto i = height - 1 ; i >= 0 ; --i) {
    for (auto j = 0U; j < width; ++j) {
      rt::Vector3f color{0, 0, 0};
      for (auto a = 0U ; a < anti_alias ; ++a) {

        auto u = static_cast<float>(j + dis(device)) / width;      
        auto v = static_cast<float>(i + dis(device)) / height;

        auto r = cam.ray(u,v);
        color += rt::ray_color(r, world);
      }
      color /= static_cast<float>(anti_alias);
      os  << static_cast<int>(color.r() * CONV) << " "
          << static_cast<int>(color.g() * CONV) << " "
          << static_cast<int>(color.b() * CONV) << " ";            
    }
    os << std::endl;
  }
}

int main(int argc, char** argv) {
  std::cout << "Ray tracing spheres" << std::endl;

  // Generate a gradient color image
  demo_draw_gradient(800, 400, "image.ppm");

  // Populate a world with Spheres
  rt::HitableList::HitablePtr worldVector;
  worldVector.push_back(std::make_unique<rt::Sphere>(rt::Vector3f{0, 0, -1}, 0.5));
  worldVector.push_back(std::make_unique<rt::Sphere>(rt::Vector3f{0, -100.5, -1}, 100));
  auto world = rt::HitableList{std::move(worldVector)};

  // Render the world
  rt::Camera cam;
  render(800, 400, world, cam, 50, "image2.ppm");
}
