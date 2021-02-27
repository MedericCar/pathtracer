#include <math.h>
#include <iostream>

#include "sphere.hh"

namespace isim {

    Sphere::Sphere(TextureMaterial* _material, Vector3 _center, float _radius)
     : Object(_material), center(_center), radius(_radius) {}
    
    bool Sphere::is_intersect(const Ray& ray) const {
        Vector3 oc = center - ray.origin;
        float d_om = oc.dot_product(ray.direction);
        float d_cm_2 = oc.dot_product(oc) - d_om * d_om;

        std::cout << d_cm_2;

        float radius2 = radius * radius;
        if (d_cm_2 > radius2)
            return false;

        float d_im = std::sqrt(radius2 - d_cm_2);
        float t0 = d_om - d_im;
        float t1 = d_om + d_im;

        if (t0 > t1)
            std::swap(t0, t1); 
 
        if (t0 < 0) { 
            t0 = t1; // if t0 is negative, let's use t1 instead 
            if (t0 < 0)
                return false; // both t0 and t1 are negative 
        } 
 
        return true; 
    }

    Vector3 Sphere::get_surface_normal(const Vector3& pos) const {
        return (pos - center).normalize();
    }

    TextureConstants Sphere::get_texture_constants(const Vector3& pos) const {
        return material->get_constants(pos);
    }
}