#pragma once

#include <math.h>
#include <random>

#include "material.hh"

namespace isim {

class MicrofacetMat : public Material {

  public:
    MicrofacetMat(Rgb kd, Rgb ks, float roughness, Rgb f0)
        : Material(Rgb(0), kd, ks, 1, Rgb(0), 0, Rgb(0), roughness), f0_(f0) {}

    Rgb eval_bsdf(const Vector3& wo, const Vector3& wi,
                  const Vector3& n) const;

  private:
      float pdf_constant_ = 1.0f;
      Rgb f0_ = Rgb(1, 0.71, 0.29); // FIXME : add other values during parsing

};

}