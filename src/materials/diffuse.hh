#pragma once

#include <math.h>
#include <random>

#include "material.hh"

namespace isim {

class DiffuseMat : public Material
{
    private:
        float pdf_constant = 1.0f / (2.0f * M_PI);

    public:
        DiffuseMat(Rgb _kd, Rgb _ke)
         : Material(Rgb(0), _kd, Rgb(0), 0, _ke, 0, Rgb(0))
        {}

        Vector3 sample(const Vector3& wo, const Vector3& n) const;

        Rgb eval_bsdf(const Vector3& wo, const Vector3& wi,
                      const Vector3& n) const;

        float pdf(const Vector3& wo, const Vector3& wi, const Vector3& n) const;
};

}