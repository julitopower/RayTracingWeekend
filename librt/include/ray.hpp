#ifndef RAY_HPP
#define RAY_HPP

#include <random>

#include <cfloat>
#include <hitable.hpp>
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
  Ray(){}

  /* Move constructor */
  Ray(Ray&& ray) : base_{std::move(ray.base_)},
                    dir_{std::move(ray.dir_)} {}

  explicit Ray(const Vector3f& base, const Vector3f& dir) :
      base_{base}, dir_{dir} {}

  /* Move assignment operator */
  Ray& operator=(Ray&& ray) {
    base_ = std::move(ray.base_);
    dir_ = std::move(ray.dir_);
    return *this;
  }

  Ray& operator=(const Ray& ray) {
    base_ = ray.base_;
    dir_ = ray.dir_;
    return *this;
  }
  
  const Vector3f& origin() const { return base_; }
  const Vector3f& dir() const { return dir_; }
  Vector3f point_at(float t) const {
    return base_ + (t * dir_);
  }
 private:
  Vector3f base_;
  Vector3f dir_;
};

Vector3f ray_color(const rt::Ray& r, const Hitable& world, int depth, bool background);

}

#endif // RAY_HPP
