#include <algorithm>

#include "../utils/random.hh"
#include "specular.hh"

namespace isim {

float fr_dielectric(float cos_theta_i, float eta_i, float eta_t) {
    cos_theta_i = std::clamp(cos_theta_i, -1.0f, 1.0f);

    // Swap indices and sign of cos if ray is in the medium
    if (cos_theta_i <= 0.f) {
        std::swap(eta_i, eta_t);
        cos_theta_i = std::abs(cos_theta_i);
    }

    float sin_theta_i = std::sqrt(std::max(0.f, 1 - cos_theta_i * cos_theta_i));
    float sin_theta_t = eta_i / eta_t * sin_theta_i;
    
    // Total reflection
    if (sin_theta_i >= 1.f) {
        return 1.f;
    }

    float cos_theta_t = std::sqrt(std::max(0.f, 1 - sin_theta_t * sin_theta_t));

    // Fresnel reflectance formulae for dielectrics : parallel and perpendicular
    // polarizations
    float r_par = ((eta_t * cos_theta_i) - (eta_i * cos_theta_t)) /
                  ((eta_t * cos_theta_i) + (eta_i * cos_theta_t));
    float r_per = ((eta_i * cos_theta_i) - (eta_t * cos_theta_t)) /
                  ((eta_i * cos_theta_i) + (eta_t * cos_theta_t));

    return (r_par * r_par + r_per * r_per) / 2;
}
        
Vector3 
SpecularMat::sample(const Vector3& wo, const Vector3& n) const {
    std::cerr << "Not implemented\n";
    exit(1);
}

Rgb SpecularMat::eval_bsdf(const Vector3& wo, const Vector3& wi,
                           const Vector3& n) const {
    std::cerr << "Not implemented\n";
    exit(1);
}

float SpecularMat::pdf(const Vector3& wo, const Vector3& wi,
                       const Vector3& n) const {
    std::cerr << "Not implemented\n";
    exit(1);
}

bool refract(const Vector3& wi, const Vector3& n, float eta, Vector3* wt) {
    float cos_theta_i = wi.dot_product(n);
    float sin2_theta_i = std::max(0.f, 1 - cos_theta_i * cos_theta_i);
    float sin2_theta_t = eta * eta * sin2_theta_i;

    // Total reflection
    if (sin2_theta_t >= 1) {
        return false;
    }

    float cos_theta_t = std::sqrt(1 - sin2_theta_t);
    *wt = wi * - 1 * eta + n * (eta * cos_theta_i - cos_theta_t);

    return true;
}

Rgb SpecularMat::sample_f(const Vector3& wo, const Vector3& n, Vector3* wi,
                          float* pdf) const {                              
    float cos_theta_o = wo.dot_product(n);
    float F = fr_dielectric(cos_theta_o, ni_i, ni_t);
    float u = random_float(0.f, 1.f);

    if (u < F) {

        *wi =  n * 2 * cos_theta_o - wo;
        float cos_theta_i = wi->dot_product(n);
        *pdf = F;
        return ks * F / std::abs(cos_theta_i);  // multiply by F bc : theta_o = thera_r

    } else {

        // If stepping out of medium
        bool entering = cos_theta_o > 0;
        float eta_i = entering ? ni_i : ni_t;
        float eta_t = entering ? ni_t : ni_i;

        Vector3 normal = n;
        if (!entering) {
            normal = normal * -1;
        }

        if (!refract(wo, normal, eta_i / eta_t, wi)) {
            return Rgb(0);
        }

        Rgb ft = kt * (1 - F);

        // TODO : account for non-symmetry when estimating from lights (BDPT)
        // ft /= (ni_i * ni_i) / (ni_t * ni_t);

        *pdf = 1 - F;

        float cos_theta_i = wi->dot_product(n);
        return ft / std::abs(cos_theta_i);
    }
}

}