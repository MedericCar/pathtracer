#pragma once

#include "image.hh"
#include "scene.hh"
#include "ray.hh"

namespace isim {

    void render(Image &img, const Scene &scene);

}