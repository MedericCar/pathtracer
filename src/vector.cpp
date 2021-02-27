#include "vector.hh"

namespace isim {

    Vector3::Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {
    }
    
    Vector3::~Vector3() {
    }
    
    std::ostream& operator<<(std::ostream &out, Vector3 &vect) {
        out << "[ " << vect.get_x() << ", " << vect.get_x()
            << ", " << vect.get_z() << " ]\n";
    }
}