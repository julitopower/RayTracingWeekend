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
#include <material.hpp>

#ifdef USE_OMP
#include <omp.h>
#endif


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

  std::vector<uint8_t> img(width * height * 3);
  for (auto i = height - 1 ; i >= 0 ; --i) {
#pragma omp parallel for
    for (auto j = 0U; j < width; ++j) {
      rt::Vector3f color{0, 0, 0};
      for (auto a = 0U ; a < anti_alias ; ++a) {

        auto u = static_cast<float>(j + dis(device)) / width;
        auto v = static_cast<float>(i + dis(device)) / height;

        auto r = cam.ray(u,v);
        color += rt::ray_color(r, world, 0);
      }
      color /= static_cast<float>(anti_alias);
      color = rt::Vector3f{float(sqrt(color.x())), float(sqrt(color.y())), float(sqrt(color.z()))};

      img[i * width * 3 + j * 3] = static_cast<uint8_t>(color.x() * CONV);
      img[i * width * 3 + j * 3 + 1] = static_cast<uint8_t>(color.y() * CONV);
      img[i * width * 3 + j * 3 + 2] = static_cast<uint8_t>(color.z() * CONV);
    }
  }

  std::cout << "here" << std::endl;
  for (auto i = height - 1 ; i >= 0 ; --i) {
    for (auto j = 0U; j < width; ++j) {    
      os << +img[i * width * 3 + j * 3] << " "
         << +img[i * width * 3 + j * 3 + 1] << " "
         << +img[i * width * 3 + j * 3 + 2] << " ";
    }
    os << std::endl;
  }
}

int main(int argc, char** argv) {
  std::cout << "Ray tracing spheres" << std::endl;

  // Generate a gradient color image
  demo_draw_gradient(800, 400, "image.ppm");

  // Register materials in the materials registry
  rt::MaterialRegistry materials{};
  materials.register_lambertian("ballgreen", {0.8, 0.8, 0});
  materials.register_lambertian("ballsalmon", {0.8, 0.3, 0.3});
  materials.register_metal("mirror", {0.8, 1, 0.5});
  materials.register_metal("perfectmirror", {0.5, 0.5, 0.5});  
  
  // Populate a world with Spheres
  rt::HitableList::HitablePtr worldVector;
  worldVector.push_back(std::make_unique<rt::Sphere>(rt::Vector3f{0, 0, -1}, 0.5,
                                                     materials.get("ballsalmon")));

  worldVector.push_back(std::make_unique<rt::Sphere>(rt::Vector3f{1, 0, -1}, 0.25,
                                                     materials.get("perfectmirror")));

  worldVector.push_back(std::make_unique<rt::Sphere>(rt::Vector3f{-2, 1, -3}, 1,
                                                     materials.get("mirror")));

  worldVector.push_back(std::make_unique<rt::Sphere>(rt::Vector3f{0, -100.5, -1}, 100,
                                                     materials.get("ballgreen")));

  auto world = rt::HitableList{std::move(worldVector)};

  // Render the world
  rt::Camera cam;
  render(800, 400, world, cam, 50, "image2.ppm");
}
