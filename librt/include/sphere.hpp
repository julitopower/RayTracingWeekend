#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <ray.hpp>
#include <vector.hpp>

#include <hitable.hpp>

namespace rt {

class Material;

class Sphere : public Hitable {
 public:
  explicit Sphere() {}
  explicit Sphere(const Vector3f center, float radius, Material* material) :
      center_{center}, radius_{radius}, material_{material} {}

  virtual bool hit(const Ray& r, float t_min, float t_max, Hit& rec) const override {
    Vector3f oc = r.origin() - center_;
    auto a = float{dot(r.dir(), r.dir())};
    auto b = float{dot(oc, r.dir())};
    auto c = float{dot(oc, oc) - radius_ * radius_};
    auto discriminant = float{b * b - a * c};
    // Set the material unconditionally
    rec.material = material_;
    if (discriminant > 0) {
      auto tmp = static_cast<float>((-b - sqrt(discriminant)) / a);
      if (tmp < t_max && tmp > t_min) {
        rec.t = tmp;
        rec.p = r.point_at(rec.t);
        rec.normal = (rec.p - center_) / radius_;
        return true;
      }
      tmp = static_cast<float>((-b + sqrt(discriminant)) / a);
      if (tmp < t_max && tmp > t_min) {
        rec.t = tmp;
        rec.p = r.point_at(rec.t);
        rec.normal = (rec.p - center_) / radius_;
        return true;
      }      
    }
    return false;
  };

 private:
  Vector3f center_;
  float radius_;
  Material* material_;
};
}
#endif //SPHERE_HPP
