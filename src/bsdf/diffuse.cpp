#include "diffuse.hh"
#include "random.hh"

namespace isim {

std::default_random_engine generator; 
std::uniform_real_distribution<float> distribution(0.0f, 1.0f); 

void createCoordinateSystem(const Vector3 &N, Vector3 &nt, Vector3 &nb) 
{ 
    if (std::fabs(N.get_x()) > std::fabs(N.get_y())) {
        nt = Vector3(N.get_z(), 0, -N.get_x()).normalize();
    }
    else {
        nt = Vector3(0, -N.get_z(), N.get_y()).normalize();
    }
    nb = cross_product(N, nt); 
} 

Sample uniform_sample_hemisphere(const Vector3& n) {
    
    float u = distribution(generator);
    float v = distribution(generator);

    float sin_theta = std::sqrt(1.0f - u * u);
    float phi = 2 * M_PI * v; 
    float x = sin_theta * std::cos(phi); 
    float z = sin_theta * std::sin(phi); 

    Vector3 dir(x, u, z);

    Vector3 nt, nb;
    createCoordinateSystem(n, nt, nb);

    Vector3 world_dir( 
        dir.x * nb.x + dir.y * n.x + dir.z * nt.x, 
        dir.x * nb.y + dir.y * n.y + dir.z * nt.y, 
        dir.x * nb.z + dir.y * n.z + dir.z * nt.z
    );

    return { world_dir, u };
}

Sample DiffuseBsdf::sample(const Vector3& wo, const Vector3& n) const { 
    return uniform_sample_hemisphere(n);    
}

float DiffuseBsdf::eval_bsdf(const Vector3& wo, const Vector3& wi,
                             const Vector3& n) const {

    return wi.dot_product(n) / M_PI;
}

float DiffuseBsdf::pdf(const Vector3& wo, const Vector3& wi,
                       const Vector3& n) const {
    return pdf_constant;
}

}