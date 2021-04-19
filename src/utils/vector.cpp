#include <iostream>
#include <math.h>

#include "vector.hh"

namespace isim {

Vector3::Vector3(float _x, float _y, float _z) : x_(_x), y_(_y), z_(_z) {}


Vector3::Vector3(const Vector3 &v) {
  x_ = v.get_x();
  y_ = v.get_y();
  z_ = v.get_z();
}


Vector3::Vector3() {
  x_ = 0, y_ = 0, z_ = 0;
}


std::ostream& operator<<(std::ostream &out, Vector3 &vect) {
  out << "[ " << vect.get_x() << ", " << vect.get_y()
      << ", " << vect.get_z() << " ]\n";
  return out;
}


std::ostream& operator<<(std::ostream &out, const Vector3 &vect) {
  out << "[ " << vect.get_x() << ", " << vect.get_y()
      << ", " << vect.get_z() << " ]\n";
  return out;
}

}