#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <ray.hpp>
#include <hitable.hpp>

namespace rt {
class Sphere : public Hitable {
 public:
  explicit Sphere() {}
  explicit Sphere(const Vector3f center, float radius) :
      center_{center}, radius_{radius} {}

  virtual bool hit(const Ray& r, float t_min, float t_max, Hit& rec) const override {
    Vector3f oc = r.origin() - center_;
    auto a = float{dot(r.dir(), r.dir())};
    auto b = float{2.0f * dot(oc, r.dir())};
    auto c = float{dot(oc, oc) - radius_ * radius_};
    auto discriminant = float{b * b - 4 * a * c};
    if (discriminant > 0) {
      auto tmp = static_cast<float>(-b - sqrt(b * b - 4 * a * c)/a);
      if (tmp < t_max && tmp > t_min) {
        rec.t = tmp;
        rec.p = r.point_at(rec.t);
        rec.normal = (rec.p - center_) / radius_;
        return true;
      }
      tmp = static_cast<float>(-b + sqrt(b * b - 4 * a * c)/a);
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
};
}
#endif //SPHERE_HPP
