#include "material.hh"
#include "../utils/random.hh"

namespace isim {

Material::Material() {
    ka = Rgb(0);
    kd = Rgb(0);
    ks = Rgb(0);
    ns = 0;
    ke = Rgb(0);
    ni_t = 0;
    ni_i = 0;
    kt = Rgb(0);
}

Material::Material(Rgb _ka , Rgb _kd, Rgb _ks, float _ns, Rgb _ke, float _ni,
                   Rgb _kt, float _roughness) 
  : ka(_ka),
    kd(_kd),
    ks(_ks),
    ns(_ns),
    ke(_ke),
    ni_t(_ni),
    kt(_kt), 
    roughness(_roughness)
{}

void createCoordinateSystem(const Vector3 &N, Vector3 &nt, Vector3 &nb) 
{ 
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
    createCoordinateSystem(n, nt, nb);

    Vector3 world_dir( 
        dir.x * nb.x + dir.y * n.x + dir.z * nt.x, 
        dir.x * nb.y + dir.y * n.y + dir.z * nt.y, 
        dir.x * nb.z + dir.y * n.z + dir.z * nt.z
    );

    return world_dir;
}

Vector3 Material::sample(const Vector3& wo, const Vector3& n) const { 
    return uniform_sample_hemisphere(n);    
}

float Material::pdf(const Vector3& wo, const Vector3& wi,
                       const Vector3& n) const {
    return pdf_constant;
}

Rgb Material::sample_f(const Vector3& wo, const Vector3& n, Vector3* wi,
                       float* pdf) const {

    *wi = sample(wo, n);
    *pdf = this->pdf(wo, *wi, n);
    return eval_bsdf(wo, *wi, n);

}
}