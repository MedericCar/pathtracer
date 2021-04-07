#include "uniform_texture.hh"

namespace isim {

    UniformTexture::UniformTexture(MaterialConstants _c) : constants(_c) {}

    MaterialConstants
    UniformTexture::get_constants(const Vector3& pos) const {
        return constants;
    }
    
}