#include <unistd.h>

#include "rendering/image.hh"
#include "rendering/render.hh"
#include "rendering/parser.hh"

int main(int argc, char *argv[]) {

  int n_samples = 64;
  std::string f;

  int c;
  while ((c = getopt(argc, argv, "f:n:")) != -1) {
    switch(c) {
      case 'f':
        f = optarg;
        continue;
      case 'n': 
        n_samples = std::stoi(optarg);
        continue;
      case -1:
        break;
    }
  }

  isim::Scene* scene = isim::load_scene(f);
  scene->summary();

  int img_h = scene->get_camera().get_height();
  int img_w = scene->get_camera().get_width();
  auto img = isim::Image(img_h, img_w);
  
  isim::render(img, *scene, n_samples);

  img.save_to_ppm("scene_json_test.ppm");

  exit(0);
}
