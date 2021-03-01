#pragma once

#include <memory>
#include <optional>

#include "texture_material.hh"
#include "ray.hh"

namespace isim {

    class Object {

    protected :
        TextureMaterial* material;

    public:
        Object(TextureMaterial* _material);
        virtual ~Object() = default;

        virtual std::optional<Vector3> is_intersect(const Ray& ray) const = 0;

        virtual Vector3 get_surface_normal(const Vector3& pos) const = 0;

        virtual TextureConstants
        get_texture_constants(const Vector3& pos) const = 0;
    };

}