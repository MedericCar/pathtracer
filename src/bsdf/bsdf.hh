#pragma once

#include "../utils/vector.hh"
#include "../rendering/ray.hh"


namespace isim {

class Bsdf
{
    public:
        Bsdf() = default;

        virtual Vector3 sample(const Vector3& wo, const Vector3& n) const = 0;

        inline Vector3 world_to_local(const Vector3& v) const {
            return Vector3(0, 0, 0);
        }

        inline Vector3 local_to_world(const Vector3& v) const {
            return Vector3(0, 0, 0);
        }
        
        virtual float
        eval_bsdf(const Vector3& wo, const Vector3& wi,
                  const Vector3& n) const = 0;

        virtual float
        pdf(const Vector3& wo, const Vector3& wi, const Vector3& n) const = 0;
};

}