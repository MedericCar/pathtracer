#pragma once

#include <memory>
#include <optional>

#include "../materials/material.hh"
#include "../rendering/ray.hh"

namespace isim {

    class Object {

    public :
        const std::string id;

    protected :
        std::shared_ptr<Material> material;

    public:
        Object(std::shared_ptr<Material> _material, const std::string& _id);
        virtual ~Object() = default;

        virtual std::optional<Vector3> is_intersect(const Ray& ray) const = 0;

        virtual inline Vector3
        get_normal(const Vector3& pos) const = 0;

        inline const Material* get_material(const Vector3& pos) const {
            return material.get();
        }

        virtual Ray sample(Vector3 pos, float& pdf) const;

    };

}