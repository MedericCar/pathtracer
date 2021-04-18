#pragma once

#include <memory>

#include "../utils/color.hh"
#include "../utils/vector.hh"

namespace isim {

  class Material {

  public:
    Material();
    Material(Rgb _ka , Rgb _kd, Rgb _ks, float _ns, Rgb _ke, float _ni,
             Rgb _kt, float _roughness);

    virtual Vector3 sample(const Vector3& wo, const Vector3& n) const;
    
    virtual Rgb eval_bsdf(const Vector3& wo, const Vector3& wi,
                          const Vector3& n) const = 0;

    virtual float pdf(const Vector3& wo, const Vector3& wi,
                      const Vector3& n) const;

    virtual Rgb sample_f(const Vector3& wo, const Vector3& n, Vector3* wi,
                         float* pdf) const;

    Rgb ka_;                  // ambient
    Rgb kd_;                  // diffusivity
    Rgb ks_;                  // specularity
    float ns_;                // specular exponent
    Rgb ke_;                  // emissive
    Rgb kt_;                  // transmittance
    float ni_t_;              // index of refraction inside material
    float ni_i_ = 1.f;        // index of refraction outside material (air)
    float roughness_ = 1.f;   // roughness (as defined in related BSDFs)

    float pdf_constant_ = 1.0f / (2.0f * M_PI);  // hemisphere sampling
                      
};
  
}