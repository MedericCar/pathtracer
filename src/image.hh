#pragma once

#include <string>

#include "color.hh"

namespace isim {

    class Image {
        
        private:
            int length;
            Rgb* pixels;

        public:
            int h, w;
            
        public:
            Image(int _h, int _w);
            ~Image();

            Rgb* get_pixels();

            void set_pixel(int i, int j, Rgb c);

            void save_to_ppm(std::string filename);
    };

}

