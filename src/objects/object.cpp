#include "object.hh"

namespace isim {

Object::Object(std::shared_ptr<Material> _material, const std::string& _id)
  : material(_material),
    id(_id)
{}

const Material& Object::get_material(const Vector3& pos) const {
    return *material;
}
    
}