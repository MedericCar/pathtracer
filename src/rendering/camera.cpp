#include <math.h>

#include "camera.hh"

namespace isim {

Vector3 const up_cam = Vector3(0, 1, 0);


Camera::Camera(const Vector3& center, const Vector3& target, float fov,
               int img_w, int img_h) 
    : center_(center),
      target_(target),
      fov_(fov),
      img_w_(img_w),
      img_h_(img_h) {            

  // FIXME : Look-At method issue when camera vertically oriented.
  Vector3 forward = (center - target).normalize();
  Vector3 right = cross_product(up_cam, forward).normalize();
  Vector3 up = cross_product(forward, right);

  camera2world_.m_[0][0] = right.get_x();
  camera2world_.m_[0][1] = right.get_y(); 
  camera2world_.m_[0][2] = right.get_z(); 
  camera2world_.m_[1][0] = up.get_x(); 
  camera2world_.m_[1][1] = up.get_y(); 
  camera2world_.m_[1][2] = up.get_z(); 
  camera2world_.m_[2][0] = forward.get_x(); 
  camera2world_.m_[2][1] = forward.get_y(); 
  camera2world_.m_[2][2] = forward.get_z(); 
  camera2world_.m_[3][0] = center.get_x(); 
  camera2world_.m_[3][1] = center.get_y(); 
  camera2world_.m_[3][2] = center.get_z(); 
}


int Camera::get_width() const {
  return img_w_;
}


int Camera::get_height() const {
  return img_h_;
}


Ray Camera::get_pixel_ray(float x, float y) const {
  float img_ratio = (float) img_w_ / (float) img_h_;
  float alpha = fov_ * M_PI / 180;
  float xp_cam = (2 * ((x + 0.5) / (float) img_w_) - 1) * tan(alpha / 2) 
                  * img_ratio;
  float yp_cam = (1 - 2 * ((y + 0.5) / (float) img_h_)) * tan(alpha / 2);

  Vector3 pixel_cam = Vector3(xp_cam, yp_cam, -1);
  Vector3 center_cam = Vector3(0, 0, 0);
  Vector3 pixel_world = camera2world_.homogeneous_mult(pixel_cam);
  Vector3 center_world = camera2world_.homogeneous_mult(center_cam);

  Ray ray = {
    .dir = (pixel_world - center_world).normalize(),
    .origin = center_world
  };

  return ray;
}

}