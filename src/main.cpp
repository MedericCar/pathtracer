#include "image.hh"
#include "render.hh"
#include "parser.hh"

int main(int argc, char const *argv[])
{
    int img_h = 1080;
    int img_w = 1920;
    auto img = isim::Image(img_h, img_w);

    isim::Scene* scene = isim::load_scene("/home/jenntedra/Documents/epita/image/s8/isim/isim-raytracer/scenes/cornell.json");
    
    isim::render(img, *scene);

    img.save_to_ppm("scene_json_test.ppm");

    exit(0);
}
