#include "gtest/gtest.h"

#include "sphere.hh"

TEST(sphere, intersect_before) {
    isim::Ray ray = {
        .dir = isim::Vector3(0, -2.9, -1.5),
        .origin = isim::Vector3(0, 3, 2)
    };

    auto texture = new isim::UniformTexture({isim::Rgb(255, 0, 0), 1, 1});
    isim::Sphere sphere(texture, isim::Vector3(0, 0, 0), 1);
    
    ASSERT_TRUE(sphere.is_intersect(ray));
}

TEST(sphere, intersect_in_center) {
    isim::Ray ray = {
        .dir = isim::Vector3(-1, -1, -1),
        .origin = isim::Vector3(2, 2, 2)
    };

    auto texture = new isim::UniformTexture({isim::Rgb(255, 0, 0), 1, 1});
    isim::Sphere sphere(texture, isim::Vector3(0, 0, 0), 1);
    
    ASSERT_TRUE(sphere.is_intersect(ray));
}

TEST(sphere, ray_after) {
    isim::Ray ray = {
        .dir = isim::Vector3(1, 1, 1),
        .origin = isim::Vector3(2, 2, 2)
    };

    auto texture = new isim::UniformTexture({isim::Rgb(255, 0, 0), 1, 1});
    isim::Sphere sphere(texture, isim::Vector3(0, 0, 0), 1);
    
    ASSERT_FALSE(sphere.is_intersect(ray));
}

TEST(sphere, ray_in) {
    isim::Ray ray = {
        .dir = isim::Vector3(1, 1, 1),
        .origin = isim::Vector3(0, 0, 0)
    };

    auto texture = new isim::UniformTexture({isim::Rgb(255, 0, 0), 1, 1});
    isim::Sphere sphere(texture, isim::Vector3(0, 0, 0), 1);
    
    ASSERT_FALSE(sphere.is_intersect(ray));
}