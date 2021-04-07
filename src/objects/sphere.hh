#pragma once 

#include "object.hh"

namespace isim {

    class Sphere : public Object {

    private:
        Vector3 center;
        float radius;

    public:
        Sphere(std::shared_ptr<Material> material, const std::string& id,
               Vector3 center, float radius);
        ~Sphere() = default;

        std::optional<Vector3> is_intersect(const Ray& ray) const;

        Vector3 get_surface_normal(const Vector3& pos) const;

        MaterialConstants get_material(const Vector3& pos) const;

    };
    
}