#include <math.h>
#include <iostream>

#include "triangle.hh"

namespace isim {

    Triangle::Triangle(std::shared_ptr<Material> _material,
                       const std::string& _id,
                       const Vector3& _pt0, 
                       const Vector3& _pt1,
                       const Vector3& _pt2)
     : Object(_material, _id), pt0(_pt0), pt1(_pt1), pt2(_pt2) {
        edge0 = _pt1 - _pt0;
        edge1 = _pt2 - _pt0;
        normal = cross_product(edge0, edge1).normalize();
    }
    
    // Möller-Trumbore algorithm : 
    //   - express problem in barycentric coordinate
    //   - reorganise equation of intersection (unknowns should be t, u, v)
    //   - solve system using Cramer's rule
    std::optional<Vector3> Triangle::is_intersect(const Ray& ray) const {
        Vector3 P = cross_product(ray.direction, edge1);
        float det = P.dot_product(edge0);

        if (det > - EPSILON && det < EPSILON)
            return std::nullopt;

        Vector3 T = ray.origin - pt0;
        float u = T.dot_product(P) * (1 / det);
        if (u < 0 || u > 1)
            return std::nullopt;

        Vector3 Q = cross_product(T, edge0);
        float v = Q.dot_product(ray.direction) * (1 / det);
        if (v < 0 || u + v > 1)
            return std::nullopt;

        float t = edge1.dot_product(Q) * (1 / det);
        
        return std::make_optional<Vector3>(ray.origin + ray.direction * t); 
    }

    Vector3 Triangle::get_surface_normal(const Vector3& pos) const {
        return normal;
    }


    MaterialConstants Triangle::get_texture_constants(const Vector3& pos) const {
        //float plane_proj = (pos - pt0).dot_product(normal);
        //if (plane_proj > -EPSILON || plane_proj < EPSILON)
        //    std::cout << (pos - pt0).dot_product(normal) << "\n";
        return material->get_constants(pos);
    }
}