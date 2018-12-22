#include <fstream>
#include <iostream>
#include <random>

#include <hitable_list.hpp>
#include <render.hpp>
#include <camera.hpp>
#include <vector.hpp>
#include <sphere.hpp>
#include <material.hpp>

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

int main(int argc, char** argv) {
  std::cout << "Ray tracing spheres" << std::endl;

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
  materials.register_light("light", {2,2,2});

  // Populate a world with Spheres
  rt::HitableList::HitablePtr worldVector;
  worldVector.push_back(std::make_unique<rt::Sphere>(rt::Vector3f{0.5, 2, -1}, 1,
						     materials.get("light")));

  worldVector.push_back(std::make_unique<rt::Sphere>(rt::Vector3f{-1, 1.5, -1}, 0.5,
						     materials.get("light")));

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

  const rt::HitableList world{std::move(worldVector)};

  // Render the world
  const rt::Vector3f lookfrom(0,2,4);
  const rt::Vector3f lookat(0,0.40,0);
  const float dist_to_focus{5.0f};
  const float aperture{0.1f};
  const rt::Vector3f vertical{0, 1, 0};

  const auto width = 800U;
  const auto height = 400U;
  const auto ratio = static_cast<float>(width) / height;

  const rt::Camera cam(lookfrom, lookat, vertical, 20, ratio,
		       aperture, dist_to_focus);

  const auto anti_alias_passes = 10U;
  rt::render(width, height, world, cam, anti_alias_passes, "image.png");
  rt::render(800, 400, random_world(materials, textures), cam,
  	     anti_alias_passes, "image_scene.png", true);
}
