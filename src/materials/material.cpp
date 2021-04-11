#include "material.hh"

namespace isim {

Material::Material(Rgb _ka , Rgb _kd, Rgb _ks, float _ns, Rgb _ke, float _ni,
                   std::shared_ptr<Bsdf> _bsdf) 
  : bsdf(_bsdf) {
    constants = {_ka, _kd, _ks, _ns, _ke, _ni};
}

Rgb Material::eval_bsdf(const Vector3& wo, const Vector3& wi,
                          const Vector3& n) const {

    return constants.kd * bsdf->eval_bsdf(wo, wi, n);
}

}