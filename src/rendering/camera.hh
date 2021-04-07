#pragma once

#include "../utils/vector.hh"
#include "../utils/matrix.hh"
#include "ray.hh"

namespace isim {

    extern Vector3 const up_cam;
    extern Vector3 const up;

    class Camera {

    private:
        Vector3 center;
        Vector3 target;
        Matrix camera2world;
        float fov;
        int img_w, img_h;

    public:
        Camera(const Vector3& center, const Vector3& target, float fov, 
               int img_w, int img_h);

        ~Camera() = default;

        int get_width() const;
        int get_height() const;

        Ray get_pixel_ray(float x, float y) const;
        Ray get_pixel_ray2(float x, float y) const;

    };

}