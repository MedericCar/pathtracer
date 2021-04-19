#include <iostream>

#include "color.hh"

namespace isim {
    
Rgb::Rgb() : r_(0), g_(0), b_(0) {} 


Rgb::Rgb(const Rgb &rgb) : r_(rgb.r_), g_(rgb.g_), b_(rgb.b_) {} 


Rgb::Rgb(float c) : r_(c), g_(c), b_(c) {} 


Rgb::Rgb(float r, float g, float b) : r_(r), g_(g), b_(b) {} 


Rgb::Rgb(const std::array<float, 3>& arr)
    : r_(arr[0]), g_(arr[1]), b_(arr[2]) {} 


bool Rgb::operator!=(const Rgb &c) const {
  return c.r_ != r_ || c.g_ != g_ || c.b_ != b_;
} 


bool Rgb::operator==(const Rgb &c) const {
  return c.r_ == r_ && c.g_ == g_ && c.b_ == b_;
} 


Rgb Rgb::clamp(float low, float high) {
  float new_r = std::clamp(r_, low, high);
  float new_g = std::clamp(g_, low, high);
  float new_b = std::clamp(b_, low, high);
  return  Rgb(new_r, new_g, new_b);
}


Rgb Rgb::operator+(const Rgb &c) const {
  float new_r = r_ + c.r_;
  float new_g = g_ + c.g_;
  float new_b = b_ + c.b_;
  return Rgb(new_r, new_g, new_b);
} 


Rgb& Rgb::operator+=(const Rgb &c) {
  r_ = r_ + c.r_;
  g_ = g_ + c.g_;
  b_ = b_ + c.b_;
  return *this;
} 


Rgb Rgb::operator-(const Rgb &c) const {
  float new_r = r_ - c.r_;
  float new_g = g_ - c.g_;
  float new_b = b_ - c.b_;
  return Rgb(new_r, new_g, new_b);
} 


Rgb& Rgb::operator-=(const Rgb &c) {
  r_ = r_ - c.r_;
  g_ = g_ - c.g_;
  b_ = b_ - c.b_;
  return *this;
} 


Rgb Rgb::operator*(float k) const {
  float new_r = r_ * k;
  float new_g = g_ * k;
  float new_b = b_ * k;
  return Rgb(new_r, new_g, new_b);
} 


Rgb& Rgb::operator*=(float k) {
  r_ = r_ * k;
  g_ = g_ * k;
  b_ = b_ * k;
  return *this;
} 


Rgb Rgb::operator*(const Rgb& other) const {
  float new_r = r_ * other.r_;
  float new_g = g_ * other.g_;
  float new_b = b_ * other.b_;
  return Rgb(new_r, new_g, new_b);
} 


Rgb& Rgb::operator*=(const Rgb& other) {
  r_ = r_ * other.r_;
  g_ = g_ * other.g_;
  b_ = b_ * other.b_;
  return *this;
} 


Rgb Rgb::operator/(float k) const {
  float new_r = r_ / k;
  float new_g = g_ / k;
  float new_b = b_ / k;
  return Rgb(new_r, new_g, new_b);
} 


Rgb& Rgb::operator/=(float k) {
  r_ /= k;
  g_ /= k;
  b_ /= k;
  return *this;
} 


std::ostream& operator<<(std::ostream &out, Rgb &c) {
  out << +c.r_ << " " << +c.g_ << " " << +c.b_ << std::endl;
  return out;
}
    

}