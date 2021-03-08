#pragma once

namespace isim { 

    class Light {
    
    public:
        Light() = default;
        virtual ~Light() = default;

        virtual Ray get_ray(const Vector3 &pt) const = 0;
    };
    
}