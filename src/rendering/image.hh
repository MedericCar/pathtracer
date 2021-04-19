#pragma once

#include <string>

#include "color.hh"

namespace isim {

class Image {
      
  public:
    Image(int h, int w);

    void set_pixel(int i, int j, Rgb c);

    void save_to_ppm(std::string filename);

    int h_, w_;
    
  private:
    int length_;
    std::vector<uint8_t> pixels_;
};

}

