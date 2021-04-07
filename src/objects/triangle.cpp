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
        edge1 = _pt1 - _pt0;
        edge2 = _pt2 - _pt0;
        normal = cross_product(edge1, edge2).normalize();
    }
    
    // Möller-Trumbore algorithm : 
    //   - express problem in barycentric coordinate P = wA + uB + vC
    //   - also : P = O + tD
    //   - reorganise equation of intersection (unknowns should be t, u, v)
    //   - solve system using Cramer's rule
    std::optional<Vector3> Triangle::is_intersect(const Ray& ray) const {
        Vector3 h = cross_product(ray.direction, edge2);
        float a = edge1.dot_product(h);

        if (a > - EPSILON && a < EPSILON)
            return std::nullopt;  // parallel ray and triangle

        float f = 1.0 / a;

        Vector3 s = ray.origin - pt0;
        float u = s.dot_product(h) * f;
        if (u < 0 || u > 1)
            return std::nullopt;

        Vector3 q = cross_product(s, edge1);
        float v = ray.direction.dot_product(q) * f;
        if (v < 0 || u + v > 1)
            return std::nullopt;  // no intersection 

        float t = edge2.dot_product(q) * f;
        if (t < EPSILON)
            return std::nullopt;  // intersection is behind camera

        return std::make_optional<Vector3>(ray.origin + ray.direction * t); 
    }

    Vector3 Triangle::get_surface_normal(const Vector3& pos) const {
        return normal;
    }

}