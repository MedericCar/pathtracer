#include "object.hh"

namespace isim {

Object::Object(std::shared_ptr<Material> _material, const std::string& _id)
  : material(_material),
    id(_id)
{}
    
}