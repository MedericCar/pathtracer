#include <algorithm>

#include "../utils/random.hh"
#include "microfacet.hh"

namespace isim {

// Surface area of microfacets aligned to the halfway vector
// Represents the randomness of microfacets directions : more/less concentrated
// reflection
float D(float NoH, float a) {
  float a2 = a * a;
  float cos2_nh = NoH * NoH;

  float denom = cos2_nh * (a2 - 1.f) + 1.f;
  denom = denom * denom * M_PI;

  return a2 / denom;
}


// Schlick approximation of the Fresnel reflectance
Rgb F(float NoH, Rgb f0) {
  return f0 + (Rgb(1) - f0) * std::pow(1.f - NoH, 5);
}


// Schlick-Beckmann approximation
float schlick_GGX(float u, float k) {
  return u / (u * (1.f - k) + k);
}


// Surface area where the micro-surface details overshadow each other (occlusion)
float G(float NoV, float NoL, float a) {
  float k = (a + 1) * (a + 1) / 8;  // remapping of alpha
  float ggx1 = schlick_GGX(NoV, k);
  float ggx2 = schlick_GGX(NoL, k);

  return ggx1 / ggx2;
}


Rgb MicrofacetMat::eval_bsdf(const Vector3& wo, const Vector3& wi,
                             const Vector3& n) const {

  Vector3 h = (wo + wi).normalize();
  float NoV = abs(n.dot_product(wo));
  float NoL = n.dot_product(wi);
  float NoH = n.dot_product(h);
  float LoH = wi.dot_product(h);

  Rgb f = F(LoH, f0_);

  //float a = roughness * roughness;
  float a = roughness_;
  float d = D(NoH, a);

  float g = G(NoV, NoL, a);

  return f * d * g  / (4 * NoV * NoL);
}

}