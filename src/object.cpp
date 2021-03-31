#include "object.hh"

namespace isim {

    Object::Object(TextureMaterial* _material, const std::string& _id)
      : material(_material),
        id(_id)
    {}
    
}