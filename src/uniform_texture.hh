#pragma once

#include "texture_material.hh"

namespace isim {

    class UniformTexture : public TextureMaterial {

    private:
        TextureConstants constants;

    public:
        UniformTexture(TextureConstants c);
        ~UniformTexture() {};

        TextureConstants get_constants(const Vector3& pos) const final;
    };
    
}