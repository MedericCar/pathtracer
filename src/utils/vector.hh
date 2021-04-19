#pragma once

#include <iostream>
#include <math.h>

namespace isim {

class Vector3 {

  public:
    Vector3(float x_, float y_, float z_);
    Vector3(const Vector3 &v);
    Vector3();
    ~Vector3() = default;

    inline float get_x() const { return x_; }
    inline float get_y() const { return y_; }
    inline float get_z() const { return z_; }

    inline float operator[](int i) const {
      switch (i) {
      case 0:
        return x_;
      case 1:
        return y_;
      case 2:
        return z_;
      default:
        std::cerr << "Index out of bounds: " << i << "\n";
        exit(1);
      }
    }

    inline Vector3 operator*(float l) const { 
      return Vector3(x_*l, y_*l, z_*l);
    }

    inline Vector3 operator+(const Vector3 &v) const {
      return Vector3(x_+v.get_x(), y_+v.get_y(), z_+v.get_z());
    }

    inline Vector3 operator-(const Vector3 &v) const {
      return Vector3(x_-v.get_x(), y_-v.get_y(), z_-v.get_z());
    }

    inline float dot_product(const Vector3 &v) const {
      return x_*v.get_x() + y_*v.get_y() + z_*v.get_z();
    }

    inline float euclidean_norm() const {
      return std::sqrt((x_ * x_) + (y_ * y_) + (z_ * z_));
    }

    inline Vector3 normalize() const {
      Vector3 copy(*this);
      return copy * (1 / euclidean_norm());
    }
    
    float x_, y_, z_;
};

inline Vector3 cross_product(const Vector3& v1, const Vector3& v2) {
  return Vector3(v1.get_y() * v2.get_z() - v1.get_z() * v2.get_y(),
                 v1.get_z() * v2.get_x() - v1.get_x() * v2.get_z(),
                 v1.get_x() * v2.get_y() - v1.get_y() * v2.get_x());
}

inline bool operator==(const Vector3& v1, const Vector3& v2) {
  return v1.x_ == v2.x_ && v1.y_ == v2.y_ && v1.z_ == v2.z_;
}

std::ostream& operator<<(std::ostream &out, Vector3 &vect);
std::ostream& operator<<(std::ostream &out, const Vector3 &vect);

}