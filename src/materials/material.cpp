#include "material.hh"

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
                   Rgb _kt) 
  : ka(_ka), kd(_kd), ks(_ks), ns(_ns), ke(_ke), ni_t(_ni), kt(_kt)
{}

Rgb Material::sample_f(const Vector3& wo, const Vector3& n, Vector3* wi,
                       float* pdf) const {

    *wi = sample(wo, n);
    *pdf = this->pdf(wo, *wi, n);
    return eval_bsdf(wo, *wi, n);

}
}