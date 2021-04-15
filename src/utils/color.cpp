#include <iostream>

#include "color.hh"

namespace isim {
    
Rgb::Rgb() : r(0), g(0), b(0) {} 

Rgb::Rgb(const Rgb &rgb) : r(rgb.r), g(rgb.g), b(rgb.b) {} 

Rgb::Rgb(float c) : r(c), g(c), b(c) {} 

Rgb::Rgb(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {} 

Rgb::Rgb(const std::array<float, 3>& arr)
    : r(arr[0]), g(arr[1]), b(arr[2])
{} 

bool Rgb::operator!=(const Rgb &c) const {
    return c.r != r || c.g != g || c.b != b;
} 

bool Rgb::operator==(const Rgb &c) const {
    return c.r == r && c.g == g && c.b == b;
} 

Rgb Rgb::clamp(float low, float high) {
    float new_r = std::clamp(r, low, high);
    float new_g = std::clamp(g, low, high);
    float new_b = std::clamp(b, low, high);
    return  Rgb(new_r, new_g, new_b);
}

Rgb Rgb::operator+(const Rgb &c) const {
    float new_r = r + c.r;
    float new_g = g + c.g;
    float new_b = b + c.b;
    return Rgb(new_r, new_g, new_b);
} 

Rgb& Rgb::operator+=(const Rgb &c) {
    r = r + c.r;
    g = g + c.g;
    b = b + c.b;
    return *this;
} 

Rgb Rgb::operator-(const Rgb &c) const {
    float new_r = r - c.r;
    float new_g = g - c.g;
    float new_b = b - c.b;
    return Rgb(new_r, new_g, new_b);
} 

Rgb& Rgb::operator-=(const Rgb &c) {
    r = r - c.r;
    g = g - c.g;
    b = b - c.b;
    return *this;
} 

Rgb Rgb::operator*(float k) const {
    float new_r = r * k;
    float new_g = g * k;
    float new_b = b * k;
    return Rgb(new_r, new_g, new_b);
} 

Rgb& Rgb::operator*=(float k) {
    r = r * k;
    g = g * k;
    b = b * k;
    return *this;
} 

Rgb Rgb::operator*(const Rgb& other) const {
    float new_r = r * other.r;
    float new_g = g * other.g;
    float new_b = b * other.b;
    return Rgb(new_r, new_g, new_b);
} 

Rgb& Rgb::operator*=(const Rgb& other) {
    r = r * other.r;
    g = g * other.g;
    b = b * other.b;
    return *this;
} 

Rgb Rgb::operator/(float k) const {
    float new_r = r / k;
    float new_g = g / k;
    float new_b = b / k;
    return Rgb(new_r, new_g, new_b);
} 

Rgb& Rgb::operator/=(float k) {
    r /= k;
    g /= k;
    b /= k;
    return *this;
} 

std::ostream& operator<<(std::ostream &out, Rgb &c) {
out << +c.r << " " << +c.g << " " << +c.b << std::endl;
return out;
}
    

}