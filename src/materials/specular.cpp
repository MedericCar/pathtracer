#include <algorithm>

#include "../utils/random.hh"
#include "specular.hh"

namespace isim {
        
// The default material methods are not implemented as the BRDF is a delta
// distribution. It is meaningless to sample it like the other.

Vector3 SpecularMat::sample(const Vector3& wo, const Vector3& n) const {
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


float fr_dielectric(float cos_theta_i, float cos_theta_t, float eta_i,
                    float eta_t) {

  // Fresnel reflectance formulae for dielectrics : parallel and perpendicular
  // polarizations
  float r_par = ((eta_t * cos_theta_i) - (eta_i * cos_theta_t)) /
                ((eta_t * cos_theta_i) + (eta_i * cos_theta_t));
  float r_per = ((eta_i * cos_theta_i) - (eta_t * cos_theta_t)) /
                ((eta_i * cos_theta_i) + (eta_t * cos_theta_t));

  return (r_par * r_par + r_per * r_per) / 2;
}


Rgb SpecularMat::sample_f(const Vector3& wo, const Vector3& n, Vector3* wi,
                          float* pdf) const {                              
  
  float cos_theta_o = wo.dot_product(n);
  Vector3 normal = n;
  float eta_i = ni_i_;
  float eta_t = ni_t_;

  // Switch if stepping out of medium
  bool entering = (cos_theta_o > 0);
  if (!entering) {
    std::swap(eta_i, eta_t);
    normal = normal * -1;
    cos_theta_o *= -1;
  }

  // Snell law to compute cos_theta_o
  float eta = eta_i / eta_t;
  float sin2_theta_o = 1 - cos_theta_o * cos_theta_o;
  float sin2_theta_i = eta * eta * sin2_theta_o;
  float cos_theta_i = std::sqrt(1 - sin2_theta_i);

  // Fresnel reflectance
  float F;
  if (sin2_theta_i >= 1.f) {
    F = 1.f;
  } else {
    F = fr_dielectric(cos_theta_o, cos_theta_i, eta_i, eta_t);
  }

  // Sample reflecting or transmitted ray with probability based on F 
  float u = random_float(0.f, 1.f);
  if (u < F) {
    *wi = normal * 2 * cos_theta_o - wo;
    *pdf = F;
    return ks_ * F / std::abs(cos_theta_i);  
  } else {
    *wi = wo * - 1 * eta + normal * (eta * cos_theta_o - cos_theta_i); 
    *pdf = 1 - F;
    return kt_ * (1 - F) / std::abs(cos_theta_i);
  }
}

}