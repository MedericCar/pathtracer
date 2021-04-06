#pragma once

#include "color.hh"
#include "vector.hh"

namespace isim {

    struct MaterialConstants {
        Rgb color;
        float kd;  // diffusivity
        float kr;  // reflectivity
        float ks;  // specularity
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