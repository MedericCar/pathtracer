#include "matrix.hh"

namespace isim {

Vector3 Matrix::homogeneous_mult(const Vector3& v) const {
  float a, b, c, w; 
  float x = v.get_x();
  float y = v.get_y();
  float z = v.get_z();

  a = x * m_[0][0] + y * m_[1][0] + z * m_[2][0] + m_[3][0]; 
  b = x * m_[0][1] + y * m_[1][1] + z * m_[2][1] + m_[3][1]; 
  c = x * m_[0][2] + y * m_[1][2] + z * m_[2][2] + m_[3][2]; 
  w = x * m_[0][3] + y * m_[1][3] + z * m_[2][3] + m_[3][3]; 

  return Vector3(a / w, b / w, c / w);
}

}