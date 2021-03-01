#pragma once 

#include "object.hh"
#include "vector.hh"

namespace isim {

    class Sphere : public Object {

    private:
        Vector3 center;
        float radius;

    public:
        Sphere(TextureMaterial *material, Vector3 center, float radius);
        ~Sphere() = default;

        std::optional<Vector3> is_intersect(const Ray& ray) const;

        Vector3 get_surface_normal(const Vector3& pos) const;

        TextureConstants get_texture_constants(const Vector3& pos) const;

    };
    
}