#include "../utils/random.hh"
#include "material.hh"

namespace isim {

Material::Material() {
  ka_ = Rgb(0);
  kd_ = Rgb(0);
  ks_ = Rgb(0);
  ns_ = 0;
  ke_ = Rgb(0);
  ni_t_ = 0;
  ni_i_ = 0;
  kt_ = Rgb(0);
}


Material::Material(Rgb ka , Rgb kd, Rgb ks, float ns, Rgb ke, float ni,
                   Rgb kt, float roughness) 
    : ka_(ka),
      kd_(kd),
      ks_(ks),
      ns_(ns),
      ke_(ke),
      ni_t_(ni),
      kt_(kt), 
      roughness_(roughness) {}


void local_basis(const Vector3 &N, Vector3 &nt, Vector3 &nb) { 
  if (std::fabs(N.get_x()) > std::fabs(N.get_y())) {
    nt = Vector3(N.get_z(), 0, -N.get_x()).normalize();
  }
  else {
    nt = Vector3(0, -N.get_z(), N.get_y()).normalize();
  }
  nb = cross_product(N, nt); 
} 


Vector3 uniform_sample_hemisphere(const Vector3& n) {
    
  float u = random_float(0.f, 1.f);
  float v = random_float(0.f, 1.f);

  float sin_theta = std::sqrt(1.0f - u * u);
  float phi = 2 * M_PI * v; 
  float x = sin_theta * std::cos(phi); 
  float z = sin_theta * std::sin(phi); 

  Vector3 dir(x, u, z);

  Vector3 nt, nb;
  local_basis(n, nt, nb);

  Vector3 world_dir( 
    dir.x_ * nb.x_ + dir.y_ * n.x_ + dir.z_ * nt.x_, 
    dir.x_ * nb.y_ + dir.y_ * n.y_ + dir.z_ * nt.y_, 
    dir.x_ * nb.z_ + dir.y_ * n.z_ + dir.z_ * nt.z_
  );

  return world_dir;
}


Vector3 Material::sample(const Vector3& wo, const Vector3& n) const { 
  return uniform_sample_hemisphere(n);    
}


float Material::pdf(const Vector3& wo, const Vector3& wi,
                    const Vector3& n) const {
  return pdf_constant_;
}


Rgb Material::sample_f(const Vector3& wo, const Vector3& n, Vector3* wi,
                       float* pdf) const {
  *wi = sample(wo, n);
  *pdf = this->pdf(wo, *wi, n);
  return eval_bsdf(wo, *wi, n);
}

}