#pragma once 

#include "object.hh"

#define EPSILON 1e-8

namespace isim {

class Triangle : public Object {

  public:
    Triangle(std::shared_ptr<Material> material, const std::string& id,
             const Vector3& pt0, const Vector3& pt1, const Vector3& pt2);

    std::optional<Vector3> is_intersect(const Ray& ray) const;

    inline Vector3 get_normal(const Vector3& pos) const {
      return normal_;
    }

    Ray sample(Vector3 pos, float& pdf) const;
      
  private:
    Vector3 pt0_, pt1_, pt2_;
    Vector3 edge1_;
    Vector3 edge2_;
    Vector3 normal_;
    float inv_area_;
    


};
    
}