#ifndef RAY_HPP
#define RAY_HPP

#include <vector.hpp>

namespace rt {

/*
 * \brief A ray is essentially a straing line with a defined origin and direction
 *
 * Any point in the straight line can be calculated as p = base + dir * t, where
 * t is a parameter
 */
class Ray {
 public:
  explicit Ray(){}
  explicit Ray(const Vector3f& base, const Vector3f& dir) :
      base_{base}, dir_{dir} {}

  const Vector3f& origin() const { return base_; }
  const Vector3f& dir() const { return dir_; }
  Vector3f point_at(float t) {
    return base_ + t * dir_;
  }
 private:
  Vector3f base_;
  Vector3f dir_;
};

bool hit_sphere(const Vector3f& center, float radius, const Ray& r) {
  Vector3f oc = r.origin() - center;
  auto a = float{dot(r.dir(), r.dir())};
  auto b = float{2.0f * dot(oc, r.dir())};
  auto c = float{dot(oc, oc) - radius * radius};
  auto discriminant = float{b * b - 4 * a * c};
  return (discriminant > 0);
}

rt::Vector3f free_ray_color(const rt::Ray r) {
  if (hit_sphere(Vector3f{0, 0, -1}, 0.5, r)) {
    return Vector3f{1, 0, 0};
  }
  const rt::Vector3f dir = rt::unit_vector(r.dir());
  auto t = float{0.5f * (dir.y() + 1.0f)};
  return (1.0 - t) * rt::Vector3f{1.0, 1.0, 1.0} + t * rt::Vector3f{0.5, 0.7, 1.0};
}

}

#endif // RAY_HPP
