#include <utility>
#include <algorithm>

#include "render.hh"

#include "sphere.hh"
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
        Rgb color = Rgb(10); 
        auto nearest_obj = find_nearest_intersection(scene.get_objects(), ray);

        // Set background
        if (!nearest_obj.has_value() && depth <= 1)
            return color;
            //return Rgb(168, 202, 255);

        // Recursion stopping case
        if (!nearest_obj.has_value() || depth == MAX_DEPTH)
            return color;

        const Object* obj = nearest_obj.value().first;
        Vector3 pos = nearest_obj.value().second;
        TextureConstants texture = obj->get_texture_constants(pos);

        Vector3 n = obj->get_surface_normal(pos);

        for (auto const& p : scene.get_lights()) {
            Ray light_ray = p->get_ray(pos);

            // Shadows
            auto light_inter = find_nearest_intersection(scene.get_objects(), 
                                                         light_ray);
            if (!light_inter || light_inter.value().first != obj)
                continue;

            // Diffuse component
            Vector3 l = light_ray.direction * -1;
            float l_dot_n = l.dot_product(n);
            color += texture.color * texture.kd * l_dot_n; 

            // Specular component
            Vector3 r = n * 2 * l_dot_n - l;
            float v_dot_r = (ray.direction * -1).dot_product(r);
            color += texture.color * texture.ks
                     * std::max(0.0, pow(v_dot_r, 5));
        }

        // Reflection
        Ray reflect_ray = Ray{
            .direction = ray.direction - n * 2 * ray.direction.dot_product(n),
            .origin = pos + n * 0.001
        };
        float k = (float)(color.r + color.g + color.b) / (255 * 3);
        //color += cast_ray(scene, reflect_ray, depth + 1) * texture.kr;
        color += cast_ray(scene, reflect_ray, depth + 1) * texture.kr * k;

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