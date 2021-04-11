#pragma once 

#include "object.hh"

#define EPSILON 1e-8

namespace isim {

    class Triangle : public Object {

    private:
        Vector3 pt0, pt1, pt2;
        Vector3 edge1;
        Vector3 edge2;
        Vector3 normal;
        float inv_area;

    public:
        Triangle(std::shared_ptr<Material> _material, const std::string& id,
                 const Vector3& _pt0, const Vector3& _pt1, const Vector3& _pt2);

        std::optional<Vector3> is_intersect(const Ray& ray) const;

        inline Vector3 get_surface_normal(const Vector3& pos) const {
            return normal;
        }

        Ray sample(Vector3 pos, float& pdf) const;

    };
    
}