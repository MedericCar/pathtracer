#pragma once

#include <math.h>
#include <random>

#include "material.hh"

namespace isim {

class SpecularMat : public Material {

  public:
    SpecularMat(Rgb ks, float ni, Rgb kt)
        : Material(Rgb(0), Rgb(0), ks, 1, Rgb(0), ni, kt, 0) {}

    Rgb sample_f(const Vector3& wo, const Vector3& n, Vector3* wi,
                 float* pdf) const;

    Vector3 sample(const Vector3& wo, const Vector3& n) const;

    Rgb eval_bsdf(const Vector3& wo, const Vector3& wi,
                  const Vector3& n) const;

    float pdf(const Vector3& wo, const Vector3& wi, const Vector3& n) const;

  private:
      float pdf_constant_ = 1.0f;

};

}