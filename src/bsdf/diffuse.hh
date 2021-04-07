#pragma once

#include <math.h>
#include <random>

#include "bsdf.hh"

namespace isim {

class DiffuseBsdf : public Bsdf
{
    private:
        float pdf = 1.0f / (2.0f * M_PI);
        std::uniform_real_distribution<float> dis;

    public:
        DiffuseBsdf() = default;

        Sample sample(const Vector3& wo, const Vector3& n) const;

        float get_pdf() const;
};

}