#pragma once

#include <memory>
#include <optional>

#include "../materials/material.hh"
#include "../rendering/ray.hh"
#include "../accelerator/aabb.hh"

namespace isim {

  class Object {

  public:
    Object(std::shared_ptr<Material> _material, const std::string& _id);
    virtual ~Object() = default;

    virtual std::optional<Vector3> is_intersect(const Ray& ray) const = 0;

    virtual inline Vector3 get_normal(const Vector3& pos) const = 0;
    
    virtual Ray sample(Vector3 pos, float& pdf) const;

    inline const Material* get_material(const Vector3& pos) const {
      return material.get();
    }

    inline virtual const AABB* get_bounding_box() const { 
      return box;
    }

    const std::string id;
    std::shared_ptr<Material> material;
    AABB* box = nullptr;

  };

}