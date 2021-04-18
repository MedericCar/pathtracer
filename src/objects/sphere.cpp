#include <math.h>
#include <iostream>

#include "sphere.hh"

namespace isim {

Sphere::Sphere(std::shared_ptr<Material> _material, const std::string& _id, 
                Vector3 _center, float _radius)
    : Object(_material, _id), center(_center), radius(_radius) {
  box = new AABB(
    center - Vector3(radius, radius, radius),
    center + Vector3(radius, radius, radius)
  );
}

std::optional<Vector3> Sphere::is_intersect(const Ray& ray) const {

  Vector3 oc = center - ray.origin;
  float d_om = oc.dot_product(ray.dir);
  float d_cm_2 = oc.dot_product(oc) - d_om * d_om;
  
  float radius2 = radius * radius;
  if (d_cm_2 > radius2) {
    return std::nullopt; // line of the ray is outside of the sphere
  }

  float d_im = std::sqrt(radius2 - d_cm_2);
  float t0 = d_om - d_im;
  float t1 = d_om + d_im;

  //if (t0 * t1 < 0) {
  //    return std::nullopt;  // ray origin is in the sphere
  //}

  if (t0 > t1) {
    std::swap(t0, t1);
  }

  if (t0 < 0) { // sign depends on position of ray to center
    t0 = t1; // if t0 is negative, let's use t1 instead 
    if (t0 < 0) {
      return std::nullopt; // t0 and t1 < 0, ray is after the sphere
    }
  }

  return std::make_optional<Vector3>(ray.origin + ray.dir * t0);
}

}