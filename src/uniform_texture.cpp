#include "uniform_texture.hh"

namespace isim {

    UniformTexture::UniformTexture(TextureConstants _c) : constants(_c) {}

    TextureConstants
    UniformTexture::get_constants(const Vector3& pos) const {
        return constants;
    }
    
}