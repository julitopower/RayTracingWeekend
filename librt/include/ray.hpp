#ifndef RAY_HPP
#define RAY_HPP

#include <random>

#include <cfloat>
#include <hitable.hpp>
#include <vector.hpp>


namespace rt {

Vector3f random_in_unit_sphere() {
  Vector3f p;
  auto dis = std::uniform_real_distribution<>{0.0, 1.0};
  std::random_device device;
  do {
    p = (2.0f * Vector3f{
      static_cast<float>(dis(device)),
      static_cast<float>(dis(device)),
          static_cast<float>(dis(device))}) - Vector3f{1, 1, 1};
  } while (p.squared_length() >= 1.0f);
  return p;
}

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
  Vector3f point_at(float t) const {
    return base_ + (t * dir_);
  }
 private:
  Vector3f base_;
  Vector3f dir_;
};

rt::Vector3f ray_color(const rt::Ray& r, const Hitable& world) {
  Hit rec;
  if (world.hit(r, 0.001, FLT_MAX, rec)) {
    Vector3f target = rec.p + rec.normal + random_in_unit_sphere();
    return 0.5 * ray_color(Ray{rec.p, target - rec.p}, world);
  } else {
    const rt::Vector3f dir = rt::unit_vector(r.dir());
    auto t = float{0.5f * (dir.y() + 1.0f)};
    return (1.0 - t) * rt::Vector3f{1.0, 1.0, 1.0} + t * rt::Vector3f{0.5, 0.7, 1.0};
  }
}

}

#endif // RAY_HPP
