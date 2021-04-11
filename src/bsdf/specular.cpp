#include <algorithm>

#include "specular.hh"

namespace isim {

float fr_dielectric(float cos_theta_i, float eta_i, float eta_t) {
    cos_theta_i = std::clamp(cos_theta_i, 1.0f, -1.0f);

    // Swap indices if ray is in the medium
    if (cos_theta_i <= 0.f) {
        std::swap(eta_i, eta_t);
        cos_theta_i *= -1;
    }

    float sin_theta_i = std::sqrt(std::max(0.f, 1 - cos_theta_i * cos_theta_i));
    float sin_theta_t = eta_i / eta_t * sin_theta_i;
    
    // Total reflection
    if (sin_theta_i) {
        return 1;
    }

    float cos_theta_t = std::sqrt(std::max(0.f, 1 - sin_theta_t * sin_theta_t));

    // Fresnel reflectance formulae for dielectrics : parallel and perpendicular polarizations
    float r_par = ((eta_t * cos_theta_i) - (eta_i * cos_theta_t)) /
                  ((eta_t * cos_theta_i) + (eta_i * cos_theta_t));
    float r_per = ((eta_i * cos_theta_i) - (eta_t * cos_theta_t)) /
                  ((eta_i * cos_theta_i) + (eta_t * cos_theta_t));

    return (r_par * r_par + r_per * r_per) / 2;
}

float SpecularBsdf::eval_bsdf(const Vector3& wo, const Vector3& wi,
                             const Vector3& n) const {

    
    return 1 / M_PI;
}

float SpecularBsdf::pdf(const Vector3& wo, const Vector3& wi,
                       const Vector3& n) const {
    return pdf_constant;
}

}