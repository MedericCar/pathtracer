#include <iostream>
#include <optional>

#include "vector.hh"
#include "image.hh"
#include "scene.hh"
#include "sphere.hh"
#include "camera.hh"
#include "vector.hh"
#include "uniform_texture.hh"
#include "render.hh"
#include "point_light.hh"

int main(int argc, char const *argv[])
{
    int img_h = 1000;
    int img_w = 1000;
    auto img = isim::Image(img_h, img_w);
    auto pixels = img.get_pixels();

    auto cam_center = isim::Vector3(0, 0, 0); 
    auto cam_target = isim::Vector3(1, 0, 0); 
    auto camera = isim::Camera(cam_center, cam_target, 90, img_w, img_h);

    auto scene = isim::Scene(camera);
    auto texture = new isim::UniformTexture({isim::Rgb(255, 0, 0), 1, 1, 0.9});
    auto texture2 = new isim::UniformTexture({isim::Rgb(255, 255, 0), 2, 1, 1});
    auto sphere = new isim::Sphere(texture, isim::Vector3(10, 0, 0), 2);
    auto sphere2 = new isim::Sphere(texture2, isim::Vector3(2, 0.5, 0), 0.5);
    scene.add_object(sphere);
    scene.add_object(sphere2);

    auto light = new isim::PointLight(isim::Vector3(0, 1, 1));
    auto light2 = new isim::PointLight(isim::Vector3(-5, 2, 10));
    //scene.add_light(light);
    scene.add_light(light2);
    
    isim::render(img, scene);

    img.save_to_ppm("oof.ppm");

    return 0;
}
