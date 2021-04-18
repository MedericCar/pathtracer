#include "object.hh"

namespace isim {

Object::Object(std::shared_ptr<Material> _material, const std::string& _id)
  : material(_material), id(_id) {}

Ray Object::sample(Vector3 pos, float& pdf) const {
    std::cerr << "Sample not yet implemented for this object.\n";
    exit(1);
}
    
}