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

float clamp(float d, float min, float max) {
    const float t = d < min ? min : d;
    return t > max ? max : t;
}

Rgb Rgb::operator+(const Rgb &c) const {
    float new_r = clamp(r + c.r, 0, 1);
    float new_g = clamp(g + c.g, 0, 1);
    float new_b = clamp(b + c.b, 0, 1);
    return Rgb(new_r, new_g, new_b);
} 

Rgb& Rgb::operator+=(const Rgb &c) {
    r = clamp((r + c.r), 0, 1);
    g = clamp((g + c.g), 0, 1);
    b = clamp((b + c.b), 0, 1);
    return *this;
} 

Rgb Rgb::operator*(float k) const {
    float new_r = clamp(r * k, 0, 1);
    float new_g = clamp(g * k, 0, 1);
    float new_b = clamp(b * k, 0, 1);
    return Rgb(new_r, new_g, new_b);
} 

Rgb& Rgb::operator*=(float k) {
    r = clamp(r * k, 0, 1);
    g = clamp(g * k, 0, 1);
    b = clamp(b * k, 0, 1);
    return *this;
} 

Rgb Rgb::operator*(const Rgb& other) const {
    float new_r = clamp(r * other.r, 0, 1);
    float new_g = clamp(g * other.g, 0, 1);
    float new_b = clamp(b * other.b, 0, 1);
    return Rgb(new_r, new_g, new_b);
} 

Rgb& Rgb::operator*=(const Rgb& other) {
    r = clamp(r * other.r, 0, 1);
    g = clamp(g * other.g, 0, 1);
    b = clamp(b * other.b, 0, 1);
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