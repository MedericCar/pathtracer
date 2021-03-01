#include "image.hh"

#include <fstream>

namespace isim {

    Image::Image(int _h, int _w) : h(_h), w(_w) {
        length = h * w;
        pixels = new Rgb[length];
    }

    Image::~Image() {
        delete pixels;
    }

    Rgb* Image::get_pixels() { 
        return pixels;
    }
    
    void Image::set_pixel(int i, int j, Rgb c) {
        pixels[i * w + j] = c;
    }

    void Image::save_to_ppm(std::string filename) {
        std::ofstream ofs(filename, std::ios::binary);

        ofs << "P6\n" << h << " " << w << "\n255\n"; 

        for (size_t i = 0; i < length; i++) {
            ofs << pixels[i].r;
            ofs << pixels[i].g;
            ofs << pixels[i].b;
        }
    }

}

