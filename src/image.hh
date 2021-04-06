#pragma once

#include <string>

#include "color.hh"

namespace isim {

    class Image {
        
        private:
            int length;
            std::vector<Rgb> pixels;

        public:
            int h, w;
            
        public:
            Image(int _h, int _w);

            void set_pixel(int i, int j, Rgb c);

            void save_to_ppm(std::string filename);
    };

}

