#include <math.h>

#include "camera.hh"

namespace isim {

    Camera::Camera(Vector3 _center, Vector3 _target, float _fov, int _img_w,
                   int _img_h) 
        : center(_center),
          target(_target),
          fov(_fov),
          img_w(_img_w),
          img_h(_img_h) {            
                
        // FIXME : Look-At method issue when camera vertically oriented.
        Vector3 forward = (center - target).normalize();
        Vector3 right = cross_product(up_world, forward).normalize();
        Vector3 up = cross_product(forward, right);

        camera2world.m[0][0] = right.get_x();
        camera2world.m[0][1] = right.get_y(); 
        camera2world.m[0][2] = right.get_z(); 
        camera2world.m[1][0] = up.get_x(); 
        camera2world.m[1][1] = up.get_y(); 
        camera2world.m[1][2] = up.get_z(); 
        camera2world.m[2][0] = forward.get_x(); 
        camera2world.m[2][1] = forward.get_y(); 
        camera2world.m[2][2] = forward.get_z(); 
        camera2world.m[3][0] = center.get_x(); 
        camera2world.m[3][1] = center.get_y(); 
        camera2world.m[3][2] = center.get_z(); 
    }

    Camera::~Camera() {
    }

    Ray Camera::get_pixel_ray(float x, float y) {
        float img_ratio = img_w / img_h;
        float alpha = fov * M_PI / 180;
        float xp_cam = (2 * ((x + 0.5) / img_w) - 1) * tan(alpha / 2) * img_ratio;
        float yp_cam = (1 - 2 * ((y + 0.5) / img_h)) * tan(alpha / 2);

        Vector3 pixel_cam = Vector3(xp_cam, yp_cam, -1);
        Vector3 center_cam = Vector3(0, 0, 0);
        Vector3 pixel_world = camera2world.homogeneous_mult(pixel_cam);
        Vector3 center_world = camera2world.homogeneous_mult(center_cam);

        Ray ray = {
            .direction = pixel_world - center_world,
            .origin = center_world
        };

        return ray;
    }




}