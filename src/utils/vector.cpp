#include <iostream>
#include <math.h>

#include "vector.hh"

namespace isim {

    Vector3::Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    Vector3::Vector3(const Vector3 &v) {
        x = v.get_x();
        y = v.get_y();
        z = v.get_z();
    }

    Vector3::Vector3() {
        x = 0, y = 0, z = 0;
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