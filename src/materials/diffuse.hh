#pragma once

#include <math.h>
#include <random>

#include "material.hh"

namespace isim {

class DiffuseMat : public Material
{
    public:
        DiffuseMat(Rgb _kd, Rgb _ke)
         : Material(Rgb(0), _kd, Rgb(0), 0, _ke, 0, Rgb(0), 0)
        {}

        Rgb eval_bsdf(const Vector3& wo, const Vector3& wi,
                      const Vector3& n) const;

};

}