#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <map>
#include <memory>

#include <vector.hpp>

namespace rt {

class Texture {
 public:
  virtual rt::Vector3f value(float u, float v, const rt::Vector3f& p) const = 0;
};

class ConstantTexture : public Texture {
 public:
  ConstantTexture() = default;
  ConstantTexture(const rt::Vector3f& c) : color_{c} {}
  rt::Vector3f value(float u, float v, const rt::Vector3f& p) const override {
    return color_;
  }  
 private:
  rt::Vector3f color_;
};

class CheckerTexture : public Texture {
 public:
  CheckerTexture() = default;
  CheckerTexture(const rt::Vector3f& c1, const rt::Vector3f& c2)
      : color1_{c1}, color2_{c2} {}

  rt::Vector3f value(float u, float v, const rt::Vector3f& p) const override {
    const auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
    return (sines < 0) ? color1_ : color2_;
  }
  
  private:
  rt::Vector3f color1_;
  rt::Vector3f color2_;
};

class LambdaTexture : public Texture {
  using Function = rt::Vector3f (*)(float, float, const rt::Vector3f&);
 public:
  LambdaTexture(Function f) : f_{f} {}
  rt::Vector3f value(float u, float v, const rt::Vector3f& p) const override {
    return f_(u, v, p);
  }
  
 private:
    Function f_;
};

class TextureRegistry {
 public:
  TextureRegistry() = default;
  void register_color(const std::string& name, const rt::Vector3f& color){
    registry_[name] = std::make_unique<ConstantTexture>(color);
  }

  template<typename T, typename... ARGS>
  void register_texture(const std::string& name, ARGS&& ...args) {
    registry_[name] = std::make_unique<T>(args...);
  }

  Texture* random_color() {
    auto dis = std::uniform_real_distribution<float>{0.0, 1.0};
    std::random_device device;
    random_.push_back(std::make_unique<ConstantTexture>(
        rt::Vector3f{dis(device) * dis(device),
              dis(device) * dis(device),
              dis(device) * dis(device)}));
    return random_.back().get();
  }

  Texture* get(const std::string& name) {
    return registry_[name].get();
  }
 private:
  std::map<std::string, std::unique_ptr<Texture>> registry_;
  std::vector<std::unique_ptr<Texture>> random_;  
};
}

#endif // TEXTURE_HPP
