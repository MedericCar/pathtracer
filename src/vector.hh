#pragma once

#include <iostream>

namespace isim {

    class Vector3 {
    
    private:
        float x, y, z;

    public:
        Vector3(float x_, float y_, float z_);
        ~Vector3() = default;

        float get_x() const { return x; }
        float get_y() const { return y; }
        float get_z() const { return z; }

        Vector3 operator*(const float &l) const;
        Vector3 operator+(const Vector3 &v) const;
        Vector3 operator-(const Vector3 &v) const;

        float euclidean_norm() const;

        float dot_product(const Vector3 &v) const;

        Vector3 normalize() const;

    };

    Vector3 cross_product(const Vector3 &v1, const Vector3 &v2);
    
    std::ostream& operator<<(std::ostream &out, Vector3 &vect);
    std::ostream& operator<<(std::ostream &out, const Vector3 &vect);

}