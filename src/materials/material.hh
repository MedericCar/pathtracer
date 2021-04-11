#pragma once

#include <memory>

#include "../utils/color.hh"
#include "../utils/vector.hh"
#include "../bsdf/bsdf.hh"

namespace isim {

    class Material {

    public:
        Rgb ka;  // ambient
        Rgb kd;  // diffusivity
        Rgb ks;  // specularity
        float ns;  // specular exponent
        Rgb ke;  // emissive
        float ni;  // index of refraction
        std::shared_ptr<Bsdf> bsdf; 

    public:
        Material(Rgb _ka , Rgb _kd, Rgb _ks, float _ns, Rgb _ke, float _ni,
                 std::shared_ptr<Bsdf> _bsdf) 
          : ka(_ka), kd(_kd), ks(_ks), ns(_ns), ke(_ke), ni(_ni), bsdf(_bsdf) 
        {}

    };
    
}