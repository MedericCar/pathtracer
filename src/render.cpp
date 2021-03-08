#include <utility>
#include <algorithm>

#include "render.hh"

#include "sphere.hh"
namespace isim {

    std::optional<std::pair<const Object*, Vector3>> 
    find_nearest_intersection(const std::vector<Object*> objects,
                              const Ray &ray) {

        std::vector<std::pair<const Object*, Vector3>> encountered;
        for (auto p : objects) {
            std::optional<Vector3> intersection = p->is_intersect(ray);
            if (intersection) {
                Vector3 pos = intersection.value();
                encountered.push_back(std::make_pair(p, pos));
            }
        }

        if (encountered.empty())
            return std::nullopt;

        auto min = std::min_element(
            encountered.begin(),
            encountered.end(),
            [](auto a, auto b) {
                return a.second.euclidean_norm() 
                        < b.second.euclidean_norm();
                } 
        );

        return std::make_optional(*min);
    }

    Rgb cast_ray(const Scene &scene, const Ray& ray, int depth) {
        Rgb color = Rgb(0); 
        auto nearest_obj = find_nearest_intersection(scene.get_objects(), ray);

        if (!nearest_obj.has_value() || depth == MAX_DEPTH)
            return color;

        const Object* obj = nearest_obj.value().first;
        Vector3 pos = nearest_obj.value().second;
        TextureConstants texture = obj->get_texture_constants(pos);

        Vector3 n = obj->get_surface_normal(pos);
        

        for (auto p : scene.get_lights()) {
            Ray light_ray = p->get_ray(pos);
            if (!obj->is_intersect(light_ray))
                continue;

            Vector3 l = light_ray.direction * -1;
            float l_dot_n = l.dot_product(n);
            Vector3 r = n * 2 * l_dot_n - l;
            float v_dot_r = (ray.direction * -1).dot_product(r);

            Ray reflect_ray = Ray{.direction=r, .origin=pos+n*0.01};
            color += cast_ray(scene, reflect_ray, depth + 1) * texture.reflectivity;
            
            color += texture.color * texture.diffusivity * l_dot_n; 
            color += texture.color * texture.specularity * std::max(0.0, pow(v_dot_r, 5));
        }

        return color;

    }

    void render(Image &img, const Scene &scene) {
        for (size_t j = 0; j < img.h; j++) {
            for (size_t i = 0; i < img.w; i++) {
                Ray view_ray = scene.get_camera().get_pixel_ray(i, j);
                Rgb color = cast_ray(scene, view_ray, 0);
                img.set_pixel(i, j, color);
            }
        }
    }
}