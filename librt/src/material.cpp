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

inline bool refract(const Vector3f& v, const Vector3f& n, float ni_over_nt, Vector3f& refracted) {
    Vector3f uv = unit_vector(v);
    float dt = dot(uv, n);
    float dis = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if (dis > 0) {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(dis);
        return true;
    } else {
        return false;
    }
}

float schlick(float cosine, float ref_idx) {
  float r0 = (1 - ref_idx) / (1 + ref_idx);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow((1 - cosine), 5);
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

bool Dielectric::scatter(const Ray& ray,
                   const Hit& rec,
                   Vector3f& attenuation,
                   Ray& scattered) const {
  Vector3f outward_normal;
  Vector3f reflected = reflect(ray.dir(), rec.normal);
  float ni_over_nt;
  attenuation = Vector3f{1.0, 1.0, 1.0};
  Vector3f refracted;
  float reflect_prob;
  float cosine;
  if (dot(ray.dir(), rec.normal) > 0) {
    outward_normal = -rec.normal;
    ni_over_nt = ref_idx_;
    cosine = ref_idx_ * dot(ray.dir(), rec.normal) / ray.dir().norm2();
  } else {
    outward_normal = rec.normal;
    ni_over_nt = 1 / ref_idx_;
    cosine = -dot(ray.dir(), rec.normal) / ray.dir().norm2();
  }

  if (refract(ray.dir(), outward_normal, ni_over_nt, refracted)) {
    reflect_prob = schlick(cosine, ref_idx_);
  } else {
    scattered = Ray{rec.p, reflected};
    reflect_prob = 1.0;
  }

  auto dis = std::uniform_real_distribution<>{0.0, 1.0};
  std::random_device device;

  if(dis(device) < reflect_prob) {
    scattered = Ray{rec.p, reflected};
  } else {
    scattered = Ray{rec.p, refracted};    
  }
  
  return true;
}

void MaterialRegistry::register_lambertian(const std::string& name,
                                           const Vector3f& attenuation) {
  registry_[name] = std::make_unique<Lambertian>(attenuation);
}

void MaterialRegistry::register_metal(const std::string& name,
                                      const Vector3f& attenuation) {
    registry_[name] = std::make_unique<Metal>(attenuation);
}

void MaterialRegistry::register_dielectric(const std::string& name,
                                           float ref_idx) {
  registry_[name] = std::make_unique<Dielectric>(ref_idx);
}

Material* MaterialRegistry::get(const std::string& name) {
  return registry_[name].get();
}
}
