#pragma once

#include "material.hh"

namespace isim {

class DiffuseMat : public Material {

  public:
    DiffuseMat(Rgb kd, Rgb ke)
        : Material(Rgb(0), kd, Rgb(0), 0, ke, 0, Rgb(0), 0) {}

    Rgb eval_bsdf(const Vector3& wo, const Vector3& wi,
                  const Vector3& n) const;

};

}