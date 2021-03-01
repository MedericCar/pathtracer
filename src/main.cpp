#include <iostream>
#include <optional>

#include "vector.hh"
#include "image.hh"
#include "scene.hh"
#include "sphere.hh"
#include "camera.hh"
#include "vector.hh"
#include "uniform_texture.hh"

int main(int argc, char const *argv[])
{
    int img_h = 1000;
    int img_w = 1000;
    auto img = isim::Image(img_h, img_w);
    auto pixels = img.get_pixels();

    auto cam_center = isim::Vector3(0, 0, 0); 
    auto cam_target = isim::Vector3(4, 0, 0); 
    auto camera = isim::Camera(cam_center, cam_target, 90, img_w, img_h);

    auto scene = isim::Scene(camera);
    auto texture = new isim::UniformTexture({isim::Rgb(255, 0, 0), 1, 1});
    auto texture2 = new isim::UniformTexture({isim::Rgb(255, 255, 0), 1, 1});
    auto sphere = new isim::Sphere(texture, isim::Vector3(4, 2, 0), 0.5);
    auto sphere2 = new isim::Sphere(texture2, isim::Vector3(4, -3.5, 0), 0.2);
    scene.add_object(sphere);
    scene.add_object(sphere2);
    
    for (size_t i = 0; i < img_h; i++) {
        for (size_t j = 0; j < img_w; j++) {
            auto ray = camera.get_pixel_ray(i, j);
            //std::cout << ray.direction;
            //std::cout << ray.origin;
            for (auto p : scene.get_objects()) {
                std::optional<isim::Vector3> intersection = p->is_intersect(ray);
                if (intersection) {
                    isim::Vector3 pos = intersection.value();
                    pixels[i * img_w + j] = p->get_texture_constants(pos).color;
                }
            }
        }
    }

    img.save_to_ppm("oof.ppm");

    return 0;
}
