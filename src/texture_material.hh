#pragma once

#include "color.hh"
#include "vector.hh"

namespace isim {

    struct TextureConstants {
        Rgb color;
        float reflectivity;
        float specularity;
    };

    class TextureMaterial {

    private:
        TextureConstants constants;
    public:
        TextureMaterial() = default;
        virtual ~TextureMaterial() = 0;

        virtual TextureConstants
        get_constants(const Vector3& pos) const = 0;
    };
    
}