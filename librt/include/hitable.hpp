#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <vector.hpp>

namespace rt {

class Material;

class Ray;

struct Hit {
  float t;
  Vector3f p;
  Vector3f normal;
  // TODO, don't use raw pointers
  Material* material;
};

class Hitable {
 public:
  virtual bool hit(const Ray& r, float t_min, float t_ma, Hit& rec) const = 0;
};
}
#endif // HITTABLE_HPP
