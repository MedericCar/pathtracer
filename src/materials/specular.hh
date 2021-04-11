#pragma once

#include <math.h>
#include <random>

#include "material.hh"

namespace isim {

class SpecularMat : public Material
{
    private:
        float pdf_constant = 1.0f;

    public:
        SpecularMat(Rgb _ks, float _ni)
         : Material(Rgb(0), Rgb(0), _ks, 1, Rgb(0), _ni)
        {}

        Vector3 sample(const Vector3& wo, const Vector3& n) const;

        Rgb eval_bsdf(const Vector3& wo, const Vector3& wi,
                        const Vector3& n) const;

        float pdf(const Vector3& wo, const Vector3& wi, const Vector3& n) const;
};

}