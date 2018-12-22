#include <ray.hpp>

#include <material.hpp>

namespace rt {

rt::Vector3f ray_color(const rt::Ray& r, const Hitable& world, int depth, bool background) {
  Hit rec;
  if (world.hit(r, 0.001, FLT_MAX, rec)) {
    Ray scattered;
    Vector3f attenuation;
    if (depth < 50 && rec.material->scatter(r, rec, attenuation, scattered)) {
      return attenuation * ray_color(scattered, world, depth + 1, background) + rec.material->emmitted();
    } else {
      return rec.material->emmitted();
    }
  } else {
    if (!background) {
	return {0,0,0};
    } else {
	Vector3f unit_dir = unit_vector(r.dir());
	float t = 0.5 * (unit_dir.y() + 1.0);
	return (1.0 - t) * rt::Vector3f{1.0, 1.0, 1.0} + t * rt::Vector3f{0.5, 0.7, 1.0};
    }
  }
}
}
