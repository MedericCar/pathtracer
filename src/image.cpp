#include "image.hh"

#include <fstream>

namespace isim {

    Image::Image(int _sx, int _sy) : sx(_sx), sy(_sy) {
        length = sx * sy;
        pixels = new Rgb[length];
    }

    Image::~Image() {
        delete pixels;
    }

    Rgb* Image::get_pixels() { 
        return pixels;
    }

    void Image::save_to_ppm(std::string filename) {
        std::ofstream ofs(filename, std::ios::binary);

        ofs << "P6\n" << sx << " " << sy << "\n255\n"; 

        for (size_t i = 0; i < length; i++) {
            ofs << pixels[i].r;
            ofs << pixels[i].g;
            ofs << pixels[i].b;
        }
    }

}

