#include <utility>
#include <algorithm>

#include "render.hh"

namespace isim {

    std::optional<std::pair<const Object*, Vector3>> 
    find_nearest_intersection(const Scene &scene, const Ray &ray) {

        std::vector<std::pair<const Object*, Vector3>> encountered;
        for (auto p : scene.get_objects()) {
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

    void render(Image &img, const Scene &scene) {
        for (size_t i = 0; i < img.h; i++) {
            for (size_t j = 0; j < img.w; j++) {

                Ray view_ray = scene.get_camera().get_pixel_ray(i, j);
                auto nearest_obj = find_nearest_intersection(scene, view_ray);

                if (!nearest_obj)
                    continue;

                const Object* obj = nearest_obj.value().first;
                Vector3 pos = nearest_obj.value().second;
                TextureConstants texture = obj->get_texture_constants(pos);

                Rgb color = Rgb(0); 

                for (auto p : scene.get_lights()) {
                    Vector3 l = p->get_ray(pos).direction;
                    Vector3 n = obj->get_surface_normal(pos);
                    float l_dot_n = l.dot_product(n);
                    Vector3 r = n - n * 2 * l_dot_n;
                    float v_dot_r = view_ray.direction.dot_product(r);
                    color += texture.color  * texture.diffusivity * l_dot_n; 
                    color += texture.color * texture.specularity * pow(v_dot_r, 3);
                }

                img.set_pixel(i, j, color);
            }
        }
    }
}