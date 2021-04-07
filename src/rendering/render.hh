#pragma once

#include "image.hh"
#include "scene.hh"
#include "ray.hh"

#define MAX_DEPTH 2

namespace isim {

    void render(Image &img, const Scene &scene);

}