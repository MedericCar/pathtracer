#pragma once

#include <memory>

#include "../utils/color.hh"
#include "../utils/vector.hh"

namespace isim {

    class Material {

    public:
        Rgb ka;  // ambient
        Rgb kd;  // diffusivity
        Rgb ks;  // specularity
        float ns;  // specular exponent
        Rgb ke;  // emissive
        Rgb kt;  // transmittance
        float ni_t;  // index of refraction inside material
        float ni_i = 1.f;  // index of refraction outside material (air)

    public:
        Material();
        Material(Rgb _ka , Rgb _kd, Rgb _ks, float _ns, Rgb _ke, float _ni,
                 Rgb _kt);

        virtual Vector3 sample(const Vector3& wo, const Vector3& n) const = 0;
        
        virtual Rgb eval_bsdf(const Vector3& wo, const Vector3& wi,
                              const Vector3& n) const = 0;

        virtual float
        pdf(const Vector3& wo, const Vector3& wi, const Vector3& n) const = 0;

        virtual Rgb sample_f(const Vector3& wo, const Vector3& n, Vector3* wi,
                             float* pdf) const;
                        
    };
    
}