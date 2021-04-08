#include "diffuse.hh"
#include "random.hh"

namespace isim {

Sample uniform_sample_hemisphere(const Vector3& n) {
    
    float u = random_float(0, 1);
    float v = random_float(0, 1);

    float z = 1 - 2 * u;
    float r = std::sqrt(std::max(0.0f, 1 - z * z));
    float phi = 2 * M_PI * v;

    Vector3 dir(r * std::cos(phi), r * std::sin(phi), z);

    // flip the vector if in the wrong way
    if (dir.dot_product(n) <= 0.0f)
        dir = dir * -1.0f;

    return { dir, u };
}

Sample DiffuseBsdf::sample(const Vector3& wo, const Vector3& n) const { 
    return uniform_sample_hemisphere(n);    
}

float DiffuseBsdf::get_pdf() const {
    return pdf;
}

}