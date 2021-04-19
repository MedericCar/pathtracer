#pragma once

#include <memory>
#include <optional>

#include "../materials/material.hh"
#include "../rendering/ray.hh"
#include "../accelerator/aabb.hh"

namespace isim {

  class Object {

  public:
    Object(std::shared_ptr<Material> material, const std::string& id);
    virtual ~Object() = default;

    virtual std::optional<Vector3> is_intersect(const Ray& ray) const = 0;

    virtual inline Vector3 get_normal(const Vector3& pos) const = 0;
    
    virtual Ray sample(Vector3 pos, float& pdf) const;

    inline const Material* get_material(const Vector3& pos) const {
      return material_.get();
    }

    inline virtual const AABB* get_bounding_box() const { 
      return box_;
    }

    const std::string id_;
    std::shared_ptr<Material> material_;
    AABB* box_ = nullptr;

  };

}