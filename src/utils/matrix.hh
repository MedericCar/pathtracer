#pragma once

#include "vector.hh"

namespace isim {

    class Matrix {

    public:
        float m[4][4] = {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1},
        };

    public:
        Matrix() = default;
        ~Matrix() = default;

        Vector3 homogeneous_mult(const Vector3& v) const;
    };

}

