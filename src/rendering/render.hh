#pragma once

#include "image.hh"
#include "scene.hh"
#include "ray.hh"

#define MAX_DEPTH 4

namespace isim {

Rgb sample_lights(const Scene& scene, const Ray& wo, const Bsdf& bsdf,
                  const Object* hit_light);

void render(Image &img, const Scene &scene);

}