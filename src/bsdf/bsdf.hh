#pragma once

#include "vector.hh"

namespace isim {

typedef struct {
    Vector3 dir;
    float cos_theta; // cos(wi,N)
} Sample;

class Bsdf
{
    private:

    public:
        Bsdf() = default;

        virtual Sample sample(const Vector3& wo, const Vector3& n) const = 0;

        virtual float get_pdf() const = 0;
};

}