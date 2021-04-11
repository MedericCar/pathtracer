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
}

Material::Material(Rgb _ka , Rgb _kd, Rgb _ks, float _ns, Rgb _ke, float _ni) 
  : ka(_ka), kd(_kd), ks(_ks), ns(_ns), ke(_ke), ni_t(_ni) {}

}