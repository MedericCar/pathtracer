#pragma once

#include <math.h>
#include <random>

#include "bsdf.hh"

namespace isim {

class DiffuseBsdf : public Bsdf
{
    private:
        float pdf_constant = 1.0f / (2.0f * M_PI);

    public:
        DiffuseBsdf() = default;

        Sample sample(const Vector3& wo, const Vector3& n) const;

        float eval_bsdf(const Vector3& wo, const Vector3& wi,
                        const Vector3& n) const;

        float pdf(const Vector3& wo, const Vector3& wi, const Vector3& n) const;
};

}