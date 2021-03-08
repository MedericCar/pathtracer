#include "point_light.hh"

namespace isim {

    PointLight::PointLight(const Vector3 &_pos) : pos(_pos) {}

    Ray PointLight::get_ray(const Vector3 &pt) const {
        return Ray{
            .direction = (pt - pos).normalize(),
            .origin = pos
        };
    }

}