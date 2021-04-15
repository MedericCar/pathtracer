#pragma once

#include <math.h>
#include <random>

#include "material.hh"

namespace isim {

class MicrofacetMat : public Material
{
    private:
        float pdf_constant = 1.0f;
        Rgb f0 = Rgb(1, 0.71, 0.29);

    public:
        MicrofacetMat(Rgb _kd, Rgb _ks, float _roughness, Rgb _f0)
         : Material(Rgb(0), _kd, _ks, 1, Rgb(0), 0, Rgb(0), _roughness),
           f0(_f0)
        {}

        Rgb eval_bsdf(const Vector3& wo, const Vector3& wi,
                      const Vector3& n) const;
};

}