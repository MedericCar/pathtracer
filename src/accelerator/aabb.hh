#pragma once

#include "../rendering/ray.hh"
#include "../utils/vector.hh"

namespace isim {

class AABB {

  public:
    AABB(const Vector3& pmin, const Vector3& pmax)
        : pmin_(pmin), pmax_(pmax) {}

    // Andrew Kensler's intersection variant
    inline bool hit(const Ray& ray, float& tmin, float& tmax) const {
      for (int a = 0; a < 3; a++) {
          float invD = 1.f / ray.dir[a];
          float t0 = (pmin_[a] - ray.origin[a]) * invD;
          float t1 = (pmax_[a] - ray.origin[a]) * invD;
          if (invD < 0.f) {
              std::swap(t0, t1);
          }

          tmin = t0 > tmin ? t0 : tmin;
          tmax = t1 < tmax ? t1 : tmax;

          if (tmin > tmax) {
              return false;
          }
      }

      return true;
    }
    
    Vector3 pmin_;
    Vector3 pmax_;

};

}