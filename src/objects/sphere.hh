#pragma once 

#include "object.hh"

namespace isim {

class Sphere : public Object {

  public:
    Sphere(std::shared_ptr<Material> material, const std::string& id,
           Vector3 center, float radius);
    ~Sphere() = default;

    std::optional<Vector3> is_intersect(const Ray& ray) const;

    inline Vector3 get_normal(const Vector3& pos) const {
      return (pos - center_).normalize();
    }

  private:
    Vector3 center_;
    float radius_;
  
};
  
}