#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include <vector>
#include <memory>

#include <hitable.hpp>
#include <ray.hpp>

namespace rt {
class HitableList : public Hitable {
 public:
  using HitablePtr = std::vector<std::unique_ptr<Hitable>>;
  
  explicit HitableList(HitablePtr&& objects) : objects_{std::move(objects)} {}
  
  virtual bool hit(const Ray& r, float t_min, float t_max, Hit& rec) const override {
    Hit tmp_hit;
    bool hit_anything = false;
    double closest = t_max;
    for(auto& hitable : objects_) {
      if (hitable->hit(r, t_min, closest, tmp_hit)) {
        hit_anything = true;
        closest = tmp_hit.t;
        rec = tmp_hit;
      }
    }
    return hit_anything;
  }
 private:
  std::vector<std::unique_ptr<Hitable>> objects_; 
};
}
#endif // HITTABLE_LIST_HPP
