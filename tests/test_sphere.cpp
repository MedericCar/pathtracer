#include "gtest/gtest.h"

#include "sphere.hh"
#include "../src/materials/material.hh"
#include "../src/materials/diffuse.hh"

auto texture = std::make_shared<isim::DiffuseMat>(
    isim::DiffuseMat(isim::Rgb(0), isim::Rgb(0))
);

TEST(sphere, intersect_before) {
    isim::Ray ray = {
        .dir = isim::Vector3(0, -2.9, -1.5),
        .origin = isim::Vector3(0, 3, 2)
    };

    isim::Sphere sphere(texture, "sphere", isim::Vector3(0, 0, 0), 1);
    
    ASSERT_TRUE(sphere.is_intersect(ray));
}

TEST(sphere, intersect_in_center) {
    isim::Ray ray = {
        .dir = isim::Vector3(-1, -1, -1),
        .origin = isim::Vector3(2, 2, 2)
    };

    isim::Sphere sphere(texture, "sphere", isim::Vector3(0, 0, 0), 1);
    
    ASSERT_TRUE(sphere.is_intersect(ray));
}

TEST(sphere, ray_after) {
    isim::Ray ray = {
        .dir = isim::Vector3(1, 1, 1),
        .origin = isim::Vector3(2, 2, 2)
    };

    isim::Sphere sphere(texture, "sphere", isim::Vector3(0, 0, 0), 1);
    
    ASSERT_FALSE(sphere.is_intersect(ray));
}

TEST(sphere, ray_in) {
    isim::Ray ray = {
        .dir = isim::Vector3(0, 0, 1),
        .origin = isim::Vector3(0, 0, 0)
    };

    isim::Sphere sphere(texture, "sphere", isim::Vector3(0, 0, 0), 1);
    
    ASSERT_TRUE(sphere.is_intersect(ray));
    ASSERT_TRUE(sphere.is_intersect(ray).value() == isim::Vector3(0, 0, 1));
}

TEST(sphere, ray_in2) {
    isim::Ray ray = {
        .dir = isim::Vector3(0, -1, 0),
        .origin = isim::Vector3(0, 0.99, 0)
    };

    isim::Sphere sphere(texture, "sphere", isim::Vector3(0, 0, 0), 1);
    
    ASSERT_TRUE(sphere.is_intersect(ray));
    ASSERT_FALSE(sphere.is_intersect(ray).value() == isim::Vector3(0, -1, 1));
    ASSERT_TRUE(sphere.is_intersect(ray).value() == isim::Vector3(0, -1, 0));
}