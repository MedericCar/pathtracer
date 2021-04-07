#pragma once

#include "light.hh"

namespace isim {

    class PointLight : public Light {
        
    private:
        Vector3 pos;

    public:
        PointLight(const Vector3 &pos);
        ~PointLight() = default;

        Ray get_ray(const Vector3 &pt) const;

    };
    

    
}