#include "diffuse.hh"

namespace isim {

Rgb DiffuseMat::eval_bsdf(const Vector3& wo, const Vector3& wi,
                          const Vector3& n) const {
  return kd_ / M_PI;
}

}