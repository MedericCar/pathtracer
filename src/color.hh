#pragma once

#include <cstdint>
#include <algorithm>
#include <math.h>
#include <iostream>

namespace isim {

    //class Rgb {
    //    
    //    public:
    //        Rgb();
    //        
    //        Rgb(uint8_t c);
    //        
    //        Rgb(uint8_t _r, uint8_t _g, uint8_t _b);
    //        
    //        bool operator!=(const Rgb &c) const {
    //            return c.r != r || c.g != g || c.b != b;
    //        } 
    //        
    //        Rgb& operator+=(const Rgb &c) {
    //            r += c.r, g += c.g, b += c.b;
    //            return *this;
    //        } 

    //        Rgb& operator*=(float k) {
    //            r *= k, g *= k, b *= k;
    //            return *this;
    //        } 
    //        
    //        uint8_t r, g, b; 
    //};

        struct Rgb {
        
        Rgb() : r(0), g(0), b(0) {} 
        
        Rgb(const Rgb &rgb) : r(rgb.r), g(rgb.g), b(rgb.b) {} 

        Rgb(uint8_t c) : r(c), g(c), b(c) {} 
        
        Rgb(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {} 
        
        bool operator!=(const Rgb &c) const {
            return c.r != r || c.g != g || c.b != b;
        } 

        bool operator==(const Rgb &c) const {
            return c.r == r && c.g == g && c.b == b;
        } 
        
        Rgb& operator+=(const Rgb &c) {
            r = std::clamp((int)std::round(r + c.r), 0, 255);
            g = std::clamp((int)std::round(g + c.g), 0, 255);
            b = std::clamp((int)std::round(b + c.b), 0, 255);
            return *this;
        } 
        
        Rgb& operator*=(float k) {
            r = std::clamp((int)std::round(r * k), 0, 255);
            g = std::clamp((int)std::round(g * k), 0, 255);
            b = std::clamp((int)std::round(b * k), 0, 255);
            return *this;
        } 

        Rgb operator*(float k) {
            uint8_t new_r = std::clamp((int)std::round(r * k), 0, 255);
            uint8_t new_g = std::clamp((int)std::round(g * k), 0, 255);
            uint8_t new_b = std::clamp((int)std::round(b * k), 0, 255);
            return Rgb(new_r, new_g, new_b);
        } 

        uint8_t r, g, b; 
    };

    std::ostream& operator<<(std::ostream &out, Rgb &c);

}