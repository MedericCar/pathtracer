#pragma once

#include "../utils/vector.hh"
#include "../utils/matrix.hh"
#include "ray.hh"

namespace isim {

extern Vector3 const up_cam;

class Camera {

  public:
    Camera(const Vector3& center, const Vector3& target, float fov, 
           int img_w, int img_h);

    ~Camera() = default;

    int get_width() const;
    int get_height() const;

    Ray get_pixel_ray(float x, float y) const;

  private:
    Vector3 center_;
    Vector3 target_;
    Matrix camera2world_;
    float fov_;
    int img_w_, img_h_;

};

}