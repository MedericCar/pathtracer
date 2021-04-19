#pragma once

#include "vector.hh"

namespace isim {

class Matrix {

  public:
    Matrix() = default;
    ~Matrix() = default;

    Vector3 homogeneous_mult(const Vector3& v) const;

    float m_[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    };

};

}

