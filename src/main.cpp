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
    int img_h = 1080;
    int img_w = 1920;
    auto img = isim::Image(img_h, img_w);
    auto pixels = img.get_pixels();

    auto cam_center = isim::Vector3(-1, 0, 1); 
    auto cam_target = isim::Vector3(0, 0, -1); 
    auto camera = isim::Camera(cam_center, cam_target, 90, img_w, img_h);

    auto scene = isim::Scene(camera);

    auto mat_ground = new isim::UniformTexture({isim::Rgb(100, 232, 104), 1, 0, 0});
    auto sphere = new isim::Sphere(mat_ground, isim::Vector3(0, -100.5, 0), 100);
    scene.add_object(sphere);

    auto mat_center = new isim::UniformTexture({isim::Rgb(167, 111, 227), 1, 0, 0.3});
    auto mat_left = new isim::UniformTexture({isim::Rgb(204, 204, 204), 1, 1, 0.7});
    auto mat_right = new isim::UniformTexture({isim::Rgb(204, 165, 50), 1, 0.7, 0.7});
    auto mat_up = new isim::UniformTexture({isim::Rgb(171, 233, 255), 1, 1, 0.7});
    auto sphere2 = new isim::Sphere(mat_center, isim::Vector3(0, 0, -1), 0.5);
    auto sphere3 = new isim::Sphere(mat_left, isim::Vector3(-1, 0, -1), 0.5);
    auto sphere4 = new isim::Sphere(mat_right, isim::Vector3(1, 0, -1), 0.5);
    auto sphere5 = new isim::Sphere(mat_up, isim::Vector3(0, 1.5, -1), 1);
    scene.add_object(sphere2);
    scene.add_object(sphere3);
    scene.add_object(sphere4);
    scene.add_object(sphere5);

    auto light = new isim::PointLight(isim::Vector3(0, 5, 10));
    auto light2 = new isim::PointLight(isim::Vector3(3, 1, 5));
    auto light3 = new isim::PointLight(isim::Vector3(-3, 1, 5));
    scene.add_light(light);
    //scene.add_light(light2);
    //scene.add_light(light3);
    
    isim::render(img, scene);

    img.save_to_ppm("oof.ppm");

    return 0;
}
