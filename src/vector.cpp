#include <math.h>

#include "vector.hh"

namespace isim {

    Vector3::Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    Vector3 Vector3::operator*(const float &l) const {
        return Vector3(x*l, y*l, z*l);
    }

    Vector3 Vector3::operator+(const Vector3 &v) const {
        return Vector3(x+v.get_x(), y+v.get_y(), z+v.get_z());
    }

    Vector3 Vector3::operator-(const Vector3 &v) const {
        return Vector3(x-v.get_x(), y-v.get_y(), z-v.get_z());
    }

    float Vector3::dot_product(const Vector3 &v) const {
        return x*v.get_x() + y*v.get_y(), z*v.get_z();
    }

    float Vector3::euclidean_norm() const {
        return std::sqrt((x * x) + (y * y) + (z * z));
    }

    Vector3 Vector3::normalize() const {
        Vector3 copy(*this);
        return copy * (1 / euclidean_norm());
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