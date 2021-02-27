#include "vector.hh"
#include "image.hh"

int main(int argc, char const *argv[])
{
    auto img = isim::Image(40, 40);
    auto pixels = img.get_pixels();
    for (size_t i = 0; i < 20; i++) {
        for (size_t j = 0; i < 40; i++)
            pixels[i * 40 + j] += isim::Rgb(100);
    }

    img.save_to_ppm("test.ppm");

    return 0;
}
