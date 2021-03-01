#include <iostream>

#include "color.hh"

namespace isim {

    std::ostream& operator<<(std::ostream &out, Rgb &c) {
       out << +c.r << " " << +c.g << " " << +c.b << std::endl;
       return out;
    }
    

}