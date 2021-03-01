#include "matrix.hh"

namespace isim {

    Matrix::Matrix() {}

    Matrix::~Matrix() {
    }
    
    Vector3 Matrix::homogeneous_mult(const Vector3& v) const {
        float a, b, c, w; 
        float x = v.get_x();
        float y = v.get_y();
        float z = v.get_z();
 
        a = x * m[0][0] + y * m[1][0] + z * m[2][0] + m[3][0]; 
        b = x * m[0][1] + y * m[1][1] + z * m[2][1] + m[3][1]; 
        c = x * m[0][2] + y * m[1][2] + z * m[2][2] + m[3][2]; 
        w = x * m[0][3] + y * m[1][3] + z * m[2][3] + m[3][3]; 

        return Vector3(a / w, b / w, c / w);
    }
}

