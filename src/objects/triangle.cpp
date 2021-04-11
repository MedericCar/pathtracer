#include <math.h>
#include <iostream>

#include "triangle.hh"
#include "../utils/random.hh"

namespace isim {

Triangle::Triangle(std::shared_ptr<Material> _material,
                    const std::string& _id,
                    const Vector3& _pt0, 
                    const Vector3& _pt1,
                    const Vector3& _pt2)
    : Object(_material, _id), pt0(_pt0), pt1(_pt1), pt2(_pt2) {
    edge1 = _pt1 - _pt0;
    edge2 = _pt2 - _pt0;
    normal = cross_product(edge1, edge2);
    inv_area = 2 / normal.euclidean_norm();
    normal = normal.normalize();
}

// Möller-Trumbore algorithm : 
//   - express problem in barycentric coordinate P = wA + uB + vC
//   - also : P = O + tD
//   - reorganise equation of intersection (unknowns should be t, u, v)
//   - solve system using Cramer's rule
std::optional<Vector3> Triangle::is_intersect(const Ray& ray) const {
    Vector3 h = cross_product(ray.dir, edge2);
    float a = edge1.dot_product(h);

    if (a > - EPSILON && a < EPSILON)
        return std::nullopt;  // parallel ray and triangle

    float f = 1.0 / a;

    Vector3 s = ray.origin - pt0;
    float u = s.dot_product(h) * f;
    if (u < 0 || u > 1)
        return std::nullopt;

    Vector3 q = cross_product(s, edge1);
    float v = ray.dir.dot_product(q) * f;
    if (v < 0 || u + v > 1)
        return std::nullopt;  // no intersection 

    float t = edge2.dot_product(q) * f;
    if (t < EPSILON)
        return std::nullopt;  // intersection is behind camera

    return std::make_optional<Vector3>(ray.origin + ray.dir * t); 
}

std::pair<float, float> sample_uniform_triangle(float u, float v)
{
    const float term = std::sqrt(u);
    return std::make_pair(1.f - term, v * term);
}
    
Ray Triangle::sample(Vector3 pos, float& pdf) const {
    pdf = inv_area;

    float u = random_float(0, 1);
    float v = random_float(0, 1);
    float e1, e2;
    std::pair<float, float> p = sample_uniform_triangle(u, v);
    e1 = p.first;
    e2 = p.second;
    Vector3 sample =  pt0 * e1 + pt1 * e2 + pt2 * (1 - e1 - e2); 

    Vector3 dir = (pos - sample).normalize();
    Vector3 origin;
    if (dir.dot_product(normal) < 0) {
        origin = sample - normal * 0.0001;
    } else {
        origin = sample + normal * 0.0001;
    }

    return { .dir = dir, .origin = origin };
}

}