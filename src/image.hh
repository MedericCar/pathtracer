#pragma once

#include <string>

#include "color.hh"

namespace isim {

    class Image {
        
        private:
            int sx, sy;
            int length;
            Rgb* pixels;
            
        public:
            Image(int _sx, int _sy);
            ~Image();

            Rgb* get_pixels();

            void save_to_ppm(std::string filename);
    };

}

