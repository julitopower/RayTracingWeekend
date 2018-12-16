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

rt::HitableList random_world(rt::MaterialRegistry& materials,
                             rt::TextureRegistry& textures) {
  rt::HitableList::HitablePtr world_vector;
  textures.register_texture<rt::CheckerTexture>("checker",
                                                rt::Vector3f{0.2, 0.3, 0.1},
                                                rt::Vector3f{0.9, 0.9, 0.9});
  
  materials.register_lambertian("floor", textures.get("checker"));
  // Floor
  world_vector.push_back(std::make_unique<rt::Sphere>(rt::Vector3f{0, -1000, 0}, 1000.f,
                                                      materials.get("floor")));

  auto dis = std::uniform_real_distribution<float>{0.0, 1.0};
  std::random_device device;  

  for(auto a = -11 ; a < 11 ; ++a) {
    for(auto b= -11 ; b < 11 ; ++b) {
      float choose_mat = dis(device);
      rt::Vector3f center{a + 0.9f * dis(device), 0.2, b + 0.9f * dis(device)};

      rt::Vector3f reference{4, 0.2, 0};
      if ((center - reference).norm2() > 0.9) {
        if (choose_mat < 0.8) {
          world_vector.push_back(
              std::make_unique<rt::Sphere>(center,
                                           0.2f,
                                           materials.generate_lambertial(textures)));
        } else if (choose_mat < 0.95) {
          world_vector.push_back(
              std::make_unique<rt::Sphere>(center,
                                           0.2f,
                                           materials.generate_metal()));          
        } else {
          world_vector.push_back(
              std::make_unique<rt::Sphere>(center,
                                           0.2f,
                                           materials.generate_dielectric()));
        }
      }
    }
  }

  world_vector.push_back(  
      std::make_unique<rt::Sphere>(rt::Vector3f{0, 1, 0},
                                   1.0f,
                                   materials.generate_dielectric()));
  world_vector.push_back(          
      std::make_unique<rt::Sphere>(rt::Vector3f{-4, 1, 0},
                                   1.0f,
                                   materials.generate_lambertial(textures)));
  world_vector.push_back(          
      std::make_unique<rt::Sphere>(rt::Vector3f{4, 1, 0},
                                   1.0f,
                                   materials.generate_metal()));    
  
  return rt::HitableList{std::move(world_vector)};
}

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
      color.sqrt();

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

  // Texture registry
  rt::TextureRegistry textures{};
  textures.register_texture<rt::ConstantTexture>("green", rt::Vector3f{0.8, 0.8, 0});
  textures.register_texture<rt::ConstantTexture>("salmon", rt::Vector3f{0.8, 0.3, 0.3});  
  // Register materials in the materials registry
  rt::MaterialRegistry materials{};
  materials.register_lambertian("ballgreen", textures.get("green"));
  materials.register_lambertian("ballsalmon", textures.get("salmon"));
  materials.register_metal("mirror", {0.8, 1, 0.5});
  materials.register_metal("perfectmirror", {0.5, 0.5, 0.5});
  materials.register_dielectric("transparent", 1.5);

  // Populate a world with Spheres
  rt::HitableList::HitablePtr worldVector;
  worldVector.push_back(std::make_unique<rt::Sphere>(rt::Vector3f{0, 0, -1}, 0.5,
                                                     materials.get("ballsalmon")));

  worldVector.push_back(std::make_unique<rt::Sphere>(rt::Vector3f{1, 0, -1}, 0.25,
                                                     materials.get("perfectmirror")));

  worldVector.push_back(std::make_unique<rt::Sphere>(rt::Vector3f{-1, 0.0, -1}, 0.5,
                                                     materials.get("transparent")));
  worldVector.push_back(std::make_unique<rt::Sphere>(rt::Vector3f{-1, 0.0, -1}, -0.45,
                                                     materials.get("transparent")));

  worldVector.push_back(std::make_unique<rt::Sphere>(rt::Vector3f{0, -100.5, -1}, 100,
                                                     materials.get("ballgreen")));

  auto world = rt::HitableList{std::move(worldVector)};

  // Render the world
  rt::Vector3f lookfrom(13,2,3);
  rt::Vector3f lookat(0,0,0);
  float dist_to_focus = 10.0;
  float aperture = 0.1;
  
  rt::Camera cam(lookfrom, lookat, {0, 1, 0},
                 20, float(800)/float(400),
                 aperture, dist_to_focus);
  
  //render(800, 400, world, cam, 200, "image2.ppm");
  render(800, 400, random_world(materials, textures), cam, 500, "image3.ppm");  
}
