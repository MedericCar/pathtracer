#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <math.h>

namespace isim {

class Rgb {

  public:
    Rgb();
    Rgb(float c);
    Rgb(float _r, float _g, float _b);
    Rgb(const Rgb& rgb); 
    Rgb(const std::array<float, 3>& arr); 
    
    bool operator!=(const Rgb& c) const;
    bool operator==(const Rgb& c) const;
    
    Rgb clamp(float a, float b);
    
    Rgb operator+(const Rgb& c) const;
    Rgb& operator+=(const Rgb& c);

    Rgb operator-(const Rgb& c) const;
    Rgb& operator-=(const Rgb& c);
    
    Rgb operator*(float k) const;
    Rgb& operator*=(float k);

    Rgb operator*(const Rgb& other) const;
    Rgb& operator*=(const Rgb& other);

    Rgb operator/(float k) const;
    Rgb& operator/=(float k);

    float r_, g_, b_; 
};

std::ostream& operator<<(std::ostream &out, Rgb &c);

}