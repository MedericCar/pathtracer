#include <math.h>
#include <iostream>

#include "triangle.hh"
#include "../utils/random.hh"

namespace isim {

Vector3 get_minimum(const Vector3 pt0, const Vector3 pt1, const Vector3 pt2) {
  return Vector3(
    std::min(pt0.x_, std::min(pt1.x_, pt2.x_)),
    std::min(pt0.y_, std::min(pt1.y_, pt2.y_)),
    std::min(pt0.z_, std::min(pt1.z_, pt2.z_))
  );
}


Vector3 get_maximum(const Vector3 pt0, const Vector3 pt1, const Vector3 pt2) {
  return Vector3(
    std::max(pt0.x_, std::max(pt1.x_, pt2.x_)),
    std::max(pt0.y_, std::max(pt1.y_, pt2.y_)),
    std::max(pt0.z_, std::max(pt1.z_, pt2.z_))
  );
}


Triangle::Triangle(std::shared_ptr<Material> material, const std::string& id,
                   const Vector3& pt0, const Vector3& pt1, const Vector3& pt2)
    : Object(material, id), pt0_(pt0), pt1_(pt1), pt2_(pt2) {
      
  edge1_ = pt1 - pt0;
  edge2_ = pt2 - pt0;
  normal_ = cross_product(edge1_, edge2_);
  inv_area_ = 2 / normal_.euclidean_norm();
  normal_ = normal_.normalize();

  Vector3 pmin = get_minimum(pt0_, pt1_, pt2_); 
  Vector3 pmax= get_maximum(pt0_, pt1_, pt2_); 
  box_ = new AABB(pmin, pmax);
}


// Möller-Trumbore algorithm : 
//   - express problem in barycentric coordinate P = wA + uB + vC
//   - also : P = O + tD
//   - reorganise equation of intersection (unknowns should be t, u, v)
//   - solve system using Cramer's rule
std::optional<Vector3> Triangle::is_intersect(const Ray& ray) const {

  Vector3 h = cross_product(ray.dir, edge2_);
  float a = edge1_.dot_product(h);

  if (a > - EPSILON && a < EPSILON) {
    return std::nullopt;  // parallel ray and triangle
  }

  float f = 1.0 / a;

  Vector3 s = ray.origin - pt0_;
  float u = s.dot_product(h) * f;
  if (u < 0 || u > 1) {
    return std::nullopt;
  }

  Vector3 q = cross_product(s, edge1_);
  float v = ray.dir.dot_product(q) * f;
  if (v < 0 || u + v > 1) {
    return std::nullopt;  // no intersection 
  }

  float t = edge2_.dot_product(q) * f;
  if (t < EPSILON) {
    return std::nullopt;  // intersection is behind camera
  }

  return std::make_optional<Vector3>(ray.origin + ray.dir * t); 
}


std::pair<float, float> sample_uniform_triangle(float u, float v) {
  const float term = std::sqrt(u);
  return std::make_pair(1.f - term, v * term);
}
    

Ray Triangle::sample(Vector3 pos, float& pdf) const {
  pdf = inv_area_;

  float u = random_float(0, 1);
  float v = random_float(0, 1);
  float e1, e2;
  std::pair<float, float> p = sample_uniform_triangle(u, v);
  e1 = p.first;
  e2 = p.second;
  Vector3 sample =  pt0_ * e1 + pt1_ * e2 + pt2_ * (1 - e1 - e2); 

  Vector3 dir = (pos - sample).normalize();
  Vector3 origin;
  if (dir.dot_product(normal_) < 0) {
    origin = sample - normal_ * 1e-7;
  } else {
    origin = sample + normal_ * 1e-7;
  }

  return { .dir = dir, .origin = origin };
}

}