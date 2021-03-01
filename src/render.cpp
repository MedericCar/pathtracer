#include <utility>
#include <algorithm>

#include "render.hh"

namespace isim {

    std::optional<std::pair<const Object*, Vector3>> 
    find_nearest_intersection(const Scene &scene, const Ray &ray) {

        std::vector<std::pair<const Object*, Vector3>> encountered;
        for (auto p : scene.get_objects()) {
            auto intersection = p->is_intersect(ray);
            if (intersection) {
                auto pos = intersection.value();
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

                auto ray = scene.get_camera().get_pixel_ray(i, j);
                auto nearest_obj = find_nearest_intersection(scene, ray);

                if (!nearest_obj)
                    continue;

                auto obj = nearest_obj.value().first;
                auto pos = nearest_obj.value().second;
                auto texture = obj->get_texture_constants(pos);

                Rgb color = Rgb(0); 

                for (auto p : scene.get_lights()) {
                    auto ray_dir = p->get_ray(pos).direction;
                    auto surface_n = obj->get_surface_normal(pos);
                    color += texture.color * (ray_dir.dot_product(surface_n));
                }

                img.set_pixel(i, j, color);
            }
        }
    }
}