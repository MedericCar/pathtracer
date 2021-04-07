#include "image.hh"

#include <fstream>

namespace isim {

    Image::Image(int _h, int _w) : h(_h), w(_w) {
        length = h * w;
        pixels = std::vector<uint8_t>(length * 3, 0);
    }

    void Image::set_pixel(int i, int j, Rgb c) {
        int index = (i + j * w) * 3;
        pixels[index] = c.r * 255;
        pixels[index + 1] = c.g * 255;
        pixels[index + 2] = c.b * 255;
    }

    void Image::save_to_ppm(std::string filename) {
        std::ofstream ofs(filename, std::ios::binary);

        ofs << "P6\n" << w << " " << h << "\n255\n"; 

        for (size_t i = 0; i < length * 3; i += 3) {
            ofs << pixels[i];
            ofs << pixels[i + 1];
            ofs << pixels[i + 2];
        }
    }

}

