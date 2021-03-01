#include <math.h>
#include <iostream>

#include "sphere.hh"

namespace isim {

    Sphere::Sphere(TextureMaterial* _material, Vector3 _center, float _radius)
     : Object(_material), center(_center), radius(_radius) {}
    
    std::optional<Vector3> Sphere::is_intersect(const Ray& ray) const {
        Vector3 oc = center - ray.origin;
        float d_om = oc.dot_product(ray.direction);
        float d_cm_2 = oc.dot_product(oc) - d_om * d_om;
        
        //std::cout << d_om << " " << d_cm_2 << "\n";

        float radius2 = radius * radius;
        if (d_cm_2 > radius2) {
            //std::cout << "outside sphere";
            return std::nullopt; // line of the ray is outside of the sphere
        }

        float d_im = std::sqrt(radius2 - d_cm_2);
        float t0 = d_om - d_im;
        float t1 = d_om + d_im;

        //std::cout << t0 << " " << t1 << "\n";

        if (t0 * t1 < 0) {
            //std::cout << "in sphere" << "\n";
            //std::cout << "OM " << oc << ray.direction << oc.dot_product(ray.direction) << "\n";
            //std::cout << "center:" << center;
            //std::cout << "radius: " << radius << "\n";
            //std::cout << "ray_ori:" << ray.direction;
            //std::cout << "t0:" << t0 << "\n";
            //std::cout << "t1:" << t1 << "\n\n";
            return std::nullopt;  // ray origin is in the sphere
        }

        if (t0 > t1)
            std::swap(t0, t1);
 
        if (t0 < 0) { // sign depends on position of ray to center
            t0 = t1; // if t0 is negative, let's use t1 instead 
            if (t0 < 0)
                return std::nullopt; // t0 and t1 < 0, ray is after the sphere
        }
 
        return std::make_optional<Vector3>(ray.origin + ray.direction * t0); 
    }

    Vector3 Sphere::get_surface_normal(const Vector3& pos) const {
        return (pos - center).normalize();
    }

    TextureConstants Sphere::get_texture_constants(const Vector3& pos) const {
        return material->get_constants(pos);
    }
}