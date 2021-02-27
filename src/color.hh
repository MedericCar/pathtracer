#pragma once

#include <cstdint>

namespace isim {

    struct Rgb {
        
        Rgb() : r(0), g(0), b(0) {} 
        
        Rgb(uint8_t c) : r(c), g(c), b(c) {} 
        
        Rgb(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {} 
        
        bool operator!=(const Rgb &c) const {
            return c.r != r || c.g != g || c.b != b;
        } 
        
        Rgb& operator+=(const Rgb &c) {
            r += c.r, g += c.g, b += c.b;
            return *this;
        } 
        
        uint8_t r, g, b; 
    };

}