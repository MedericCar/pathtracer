#pragma once

#include "material.hh"

namespace isim {

    class UniformTexture : public Material {

    private:
        MaterialConstants constants;

    public:
        UniformTexture(MaterialConstants c);
        ~UniformTexture() {};

        MaterialConstants get_constants(const Vector3& pos) const final;
    };
    
}