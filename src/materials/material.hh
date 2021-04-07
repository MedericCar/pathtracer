#pragma once

#include <memory>

#include "../utils/color.hh"
#include "../utils/vector.hh"
#include "../bsdf/bsdf.hh"

namespace isim {

    struct MaterialConstants {
        Rgb ka;  // ambient
        Rgb kd;  // diffusivity
        Rgb ks;  // specularity
        float ns;  // specular exponent
        Rgb ke;  // emissive
        float illumination;
        std::shared_ptr<Bsdf> bsdf; 
    };

    class Material {

    private:
        MaterialConstants constants;
    public:
        Material() = default;
        virtual ~Material() = default;

        virtual MaterialConstants
        get_constants(const Vector3& pos) const = 0;
    };
    
}