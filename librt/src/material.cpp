#include <material.hpp>
#include <ray.hpp>
#include <vector.hpp>

namespace rt{
namespace {
/*
 * Specular reflection
 */
inline Vector3f reflect(const Vector3f& v, const Vector3f& n) {
  return v - 2 * dot(v, n) * n;
}
} // Unnamed namespace

bool Lambertian::scatter(const Ray& /* ray */,
                         const Hit& rec,
                         Vector3f& attenuation,
                         Ray& scattered) const {
  Vector3f target = rec.p + rec.normal + random_in_unit_sphere();
  scattered = Ray{rec.p, target - rec.p};
  attenuation = albedo_;
  return true;
}

bool Metal::scatter(const Ray& ray,
                   const Hit& rec,
                   Vector3f& attenuation,
                   Ray& scattered) const {
  Vector3f reflected = reflect(unit_vector(ray.dir()), rec.normal);
  scattered = Ray{rec.p, reflected};
  attenuation = albedo_;
  return dot(scattered.dir(), rec.normal) > 0;
}

void MaterialRegistry::register_lambertian(const std::string& name,
                                           const Vector3f& attenuation) {
  registry_[name] = std::make_unique<Lambertian>(attenuation);
}
  
void MaterialRegistry::register_metal(const std::string& name,
                                      const Vector3f& attenuation) {
    registry_[name] = std::make_unique<Metal>(attenuation);
}

Material* MaterialRegistry::get(const std::string& name) {
  return registry_[name].get();
}
}
