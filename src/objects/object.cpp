#include "object.hh"

namespace isim {

Object::Object(std::shared_ptr<Material> material, const std::string& id)
    : material_(material), id_(id) {}


Ray Object::sample(Vector3 pos, float& pdf) const {
  std::cerr << "Sample not yet implemented for this object.\n";
  exit(1);
}
    
}