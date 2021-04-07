#pragma once

#include <cstdint>
#include <algorithm>
#include <math.h>
#include <iostream>

namespace isim {

    class Rgb {

    public:
        Rgb();
        Rgb(uint8_t c);
        Rgb(uint8_t _r, uint8_t _g, uint8_t _b);
        Rgb(const Rgb &rgb); 
        
        bool operator!=(const Rgb &c) const;
        bool operator==(const Rgb &c) const;
        
        Rgb operator+(const Rgb &c);
        Rgb& operator+=(const Rgb &c);
        
        Rgb operator*(float k) const;
        Rgb& operator*=(float k);

        uint8_t r, g, b; 
    };

    std::ostream& operator<<(std::ostream &out, Rgb &c);

}