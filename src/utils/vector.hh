#pragma once

#include <iostream>
#include <math.h>

namespace isim {

    class Vector3 {
    
    private:
        float x, y, z;

    public:
        Vector3(float x_, float y_, float z_);
        Vector3(const Vector3 &v);
        Vector3();
        ~Vector3() = default;

        inline float get_x() const { return x; }
        inline float get_y() const { return y; }
        inline float get_z() const { return z; }

        inline Vector3 operator*(float l) const { 
            return Vector3(x*l, y*l, z*l);
        }

        inline Vector3 operator+(const Vector3 &v) const {
            return Vector3(x+v.get_x(), y+v.get_y(), z+v.get_z());
        }

        inline Vector3 operator-(const Vector3 &v) const {
            return Vector3(x-v.get_x(), y-v.get_y(), z-v.get_z());
        }

        inline float dot_product(const Vector3 &v) const {
            return x*v.get_x() + y*v.get_y() + z*v.get_z();
        }

        inline float euclidean_norm() const {
            return std::sqrt((x * x) + (y * y) + (z * z));
        }

        inline Vector3 normalize() const {
            Vector3 copy(*this);
            return copy * (1 / euclidean_norm());
        }

    };

    inline Vector3 cross_product(const Vector3& v1, const Vector3& v2) {
        return Vector3(v1.get_y() * v2.get_z() - v1.get_z() * v2.get_y(),
                        v1.get_z() * v2.get_x() - v1.get_x() * v2.get_z(),
                        v1.get_x() * v2.get_y() - v1.get_y() * v2.get_x());
    }
    
    std::ostream& operator<<(std::ostream &out, Vector3 &vect);
    std::ostream& operator<<(std::ostream &out, const Vector3 &vect);

}