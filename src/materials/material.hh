#pragma once

#include <memory>

#include "../utils/color.hh"
#include "../utils/vector.hh"
#include "../bsdf/bsdf.hh"

namespace isim {

    typedef struct {
        Rgb ka;  // ambient
        Rgb kd;  // diffusivity
        Rgb ks;  // specularity
        float ns;  // specular exponent
        Rgb ke;  // emissive
        float ni;  // index of refraction
    } MaterialConstants;


    class Material {

    public:
        MaterialConstants constants;
        std::shared_ptr<Bsdf> bsdf; 

    public:
        Material(Rgb _ka , Rgb _kd, Rgb _ks, float _ns, Rgb _ke, float _ni,
                 std::shared_ptr<Bsdf> _bsdf);

        Rgb eval_bsdf(const Vector3& wo, const Vector3& wi,
                      const Vector3& n) const;
                        
    };
    
}