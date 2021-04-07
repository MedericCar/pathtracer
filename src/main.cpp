#include "rendering/image.hh"
#include "rendering/render.hh"
#include "rendering/parser.hh"

int main(int argc, char const *argv[])
{
    std::string f = "/home/jenntedra/Documents/epita/image/s8/isim/isim-raytracer/scenes/cornell.json";
    isim::Scene* scene = isim::load_scene(f);
    scene->summary();

    int img_h = scene->get_camera().get_height();
    int img_w = scene->get_camera().get_width();
    auto img = isim::Image(img_h, img_w);
    
    isim::render(img, *scene);

    img.save_to_ppm("scene_json_test.ppm");

    exit(0);
}
