#pragma once

#include "vector.hh"
#include "matrix.hh"
#include "ray.hh"

namespace isim {

    Vector3 up_world = Vector3(0, 1, 0);

    class Camera {

    private:
        Vector3 center;
        Vector3 target;
        Matrix camera2world;
        float fov;
        int img_w, img_h;

    public:
        Camera(Vector3 center, Vector3 target, float fov, int img_w, int img_h);

        ~Camera();

        Ray get_pixel_ray(float x, float y);

    };

}