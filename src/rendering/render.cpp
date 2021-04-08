#include <utility>
#include <algorithm>

#include "render.hh"

namespace isim {

std::optional<std::pair<Object const*, Vector3>> 
find_nearest_intersection(const std::vector<std::unique_ptr<Object>>& objects,
                            const Ray &ray) {

    std::vector<std::pair<Object const*, Vector3>> encountered;
    for (auto const& p : objects) {
        std::optional<Vector3> intersection = p->is_intersect(ray);
        if (intersection) {
            Vector3 pos = intersection.value();
            encountered.push_back(std::make_pair(p.get(), pos));
        }
    }

    if (encountered.empty())
        return std::nullopt;

    auto min = std::min_element(
        encountered.begin(),
        encountered.end(),
        [&ray](auto a, auto b) {
            return (a.second - ray.origin).euclidean_norm() 
                    < (b.second - ray.origin).euclidean_norm();
            } 
    );

    return std::make_optional(*min);
}

Rgb cast_ray(const Scene &scene, const Ray& ray, int depth) {
    Rgb color = Rgb(10/255); 
    auto nearest_obj = find_nearest_intersection(scene.get_objects(), ray);

    // Set background
    if (!nearest_obj.has_value() && depth <= 1)
        return Rgb(0.66, 0.79, 1);
        //return color;

    // Recursion stopping case
    if (!nearest_obj.has_value() || depth == MAX_DEPTH)
        return color;

    const Object* obj = nearest_obj.value().first;
    Vector3 pos = nearest_obj.value().second;
    MaterialConstants material = obj->get_material(pos).get_constants(pos);

    Vector3 n = obj->get_surface_normal(pos);

    for (auto const& p : scene.get_lights()) {
        Ray light_ray = p->get_ray(pos);

        // Shadows
        auto light_inter = find_nearest_intersection(scene.get_objects(), 
                                                     light_ray);
        if (!light_inter || light_inter.value().first != obj)
            continue;

        // Ambient component
        color = material.ka * material.ke;

        // Diffuse component
        Vector3 l = light_ray.direction * -1;
        float l_dot_n = l.dot_product(n);
        color += material.kd * l_dot_n; 

        // Specular component
        Vector3 r = n * 2 * l_dot_n - l;
        float v_dot_r = (ray.direction * -1).dot_product(r);
        color += material.ks * std::max(0.0, pow(v_dot_r, (int) material.ns));
    }

    // Reflection
    Ray reflect_ray = Ray{
        .direction = ray.direction - n * 2 * ray.direction.dot_product(n),
        .origin = pos + n * 0.001
    };

    float k = (color.r + color.g + color.b) / 3;
    color += cast_ray(scene, reflect_ray, depth + 1) * material.ks * k;

    return color;
}

Rgb path_trace(const Scene &scene, const Ray& ray, int depth) {
    Rgb color = Rgb(0.0f); 
    auto nearest_obj = find_nearest_intersection(scene.get_objects(), ray);

    // Recursion stopping case
    if (!nearest_obj.has_value())
        return color;

    const Object* obj = nearest_obj.value().first;
    Vector3 pos = nearest_obj.value().second;
    MaterialConstants material = obj->get_material(pos).get_constants(pos);
    Vector3 n = obj->get_surface_normal(pos);

    // Add direct light
    Rgb direct = material.ka * material.ke;
       
    if (depth == MAX_DEPTH || direct != Rgb(0)) {
        return direct;
    }

    // Add inddirect light by sampling
    Rgb indirect = Rgb(0);
    int n_samples = 128;  // FIXME
    float inv_samples = 1.0f / n_samples;
    float inv_pdf = 1.0f / material.bsdf->get_pdf();
    
    for (int i = 0; i < n_samples; i++) {
        Sample s = material.bsdf->sample(ray.direction, n);
        Ray sample_ray = { .direction = s.dir, .origin = pos }; 
        indirect += material.kd * path_trace(scene, sample_ray, depth + 1) * cos(s.cos_theta) * inv_pdf * inv_samples;
        //std::cout << indirect;
    }


    //indirect *= inv_samples * inv_pdf;
    color = direct + indirect;
    //std::cout << "FINAL : " << color;
    //color = direct + indirect * (1 / (n_samples * material.bsdf->get_pdf())); 


    return color;
}

void render(Image &img, const Scene &scene) {
    int n = img.h * img.w;
    float k = 0;

    for (size_t j = 0; j < img.h; j++) {

        for (size_t i = 0; i < img.w; i++) {

            if (j * img.w + i > (k / 100) * n) {
                std::cout << j * img.w + i << " / " << n << " - ";
                std::cout << k + 1 << " %\n";
                k++;
            }
            //std::cout << i << " " << j << "\n";
            Ray view_ray = scene.get_camera().get_pixel_ray(i, j);
            //Rgb color = cast_ray(scene, view_ray, 0);
            Rgb color = path_trace(scene, view_ray, 0);
            //std::cout << "color : " << color << "\n";
            img.set_pixel(i, j, color);
        }
    }
    std::cout << "Im out" << "\n";
}

}