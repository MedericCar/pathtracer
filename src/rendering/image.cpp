#include "image.hh"

#include <fstream>

namespace isim {

Image::Image(int h, int w) : h_(h), w_(w) {
  length_ = h_ * w_;
  pixels_ = std::vector<uint8_t>(length_ * 3, 0);
}


void Image::set_pixel(int i, int j, Rgb c) {
  int index = (i + j * w_) * 3;
  pixels_[index] = c.r_ * 255;
  pixels_[index + 1] = c.g_ * 255;
  pixels_[index + 2] = c.b_ * 255;
}


void Image::save_to_ppm(std::string filename) {
  std::ofstream ofs(filename, std::ios::binary);

  ofs << "P6\n" << w_ << " " << h_ << "\n255\n"; 

  for (size_t i = 0; i < length_ * 3; i += 3) {
    ofs << pixels_[i];
    ofs << pixels_[i + 1];
    ofs << pixels_[i + 2];
  }
}

}

