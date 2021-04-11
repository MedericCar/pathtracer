#include <utility>
#include <algorithm>
#include <thread>
#include <atomic>

#include "render.hh"
#include "../utils/random.hh"

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

Rgb radiance(const Scene &scene, const Ray& ray, int depth) {
    Rgb color = Rgb(0.0f); 
    auto nearest_obj = find_nearest_intersection(scene.get_objects(), ray);

    // Recursion stopping case
    if (!nearest_obj.has_value())
        return color;

    const Object* obj = nearest_obj.value().first;
    Vector3 pos = nearest_obj.value().second;
    MaterialConstants mat = obj->get_material(pos).get_constants(pos);
    Vector3 n = obj->get_surface_normal(pos);

    // Add direct light
    Rgb direct = mat.ke;
    if (depth == MAX_DEPTH || direct != Rgb(0)) {
        return direct;
    }

    // Add indirect light by sampling
    Rgb indirect = Rgb(0);
    Sample s = mat.bsdf->sample(ray.direction, n);
    Ray sample_ray = { .direction = s.dir, .origin = pos + s.dir * 0.001 }; 

    float pdf = mat.bsdf->pdf(ray.direction, s.dir, n);
    float bsdf = mat.bsdf->eval_bsdf(ray.direction, s.dir, n);
    indirect += mat.kd * radiance(scene, sample_ray, depth + 1) * bsdf / pdf;

    color = direct + indirect;

    return color;
}

Rgb sample_lights(const Scene& scene, const Ray& wo, const Bsdf& bsdf,
                  const Vector3& pos, const Vector3& n, const Object* target_obj,
                  const Object* hit_light) {

    Rgb Ld(0);
    for (auto const& ptr : scene.get_objects()) {  // BIG FIXME IF THERE ARE MANY OBJECTS RIP
        const Object* l = ptr.get();
        MaterialConstants mat = l->get_material(pos).get_constants(pos);
        if (l == hit_light || mat.ke == Rgb(0)) {
            continue;
        }

        float pdf = 0.0f;
        Ray wi = l->sample(pos, pdf);

        auto nearest_obj = find_nearest_intersection(scene.get_objects(), wi);
        if (!nearest_obj.has_value() || nearest_obj.value().first != target_obj)
            continue;

        float f = bsdf.eval_bsdf(wo.direction, wi.direction * -1, n);
        if (f == 0 || pdf == 0.f)
            continue;
        
        float square_norm = (wi.origin - pos).euclidean_norm();
        square_norm *= square_norm;

        Vector3 light_n = l->get_surface_normal(wi.origin);  // FIXME could save beforehand
        float cos1 = light_n.dot_product(wi.direction);
        float cos2 = n.dot_product(wi.direction * -1);

        //Ld += mat.ke * f * std::fmax(n.dot_product(wi.direction * -1), 0) / pdf2;
        //Ld += mat.ke * f * std::abs(n.dot_product(wi.direction * -1)) / pdf2;
        Ld += mat.ke * f * cos1 * cos2 / (square_norm * pdf);
        //std::cout << f << " " << Ld;
    }

    //if (Ld != Rgb(0))
    //    std::cout << "Ld computed\n";

    return Ld;
}

Rgb path_trace_pbr(const Scene &scene, Ray wo) {
    Rgb L(0.0f); 
    Rgb throughput(1.0f);
    bool specular_bounce = false;

    for (int bounces = 0; ; bounces++) {

        auto nearest_obj = find_nearest_intersection(scene.get_objects(), wo);
        if (!nearest_obj.has_value() || bounces > MAX_DEPTH) {
            L += throughput * Rgb(0);
            break;
        }

        // Recover intersection's info
        const Object* obj = nearest_obj.value().first;
        Vector3 pos = nearest_obj.value().second;
        Vector3 n = obj->get_surface_normal(pos);
        MaterialConstants mat = obj->get_material(pos).get_constants(pos);

        // Intersection with emissive object : two exceptions for adding
        const Object* hit_light = nullptr;
        if (mat.ke != Rgb(0)) {
            hit_light = obj;
            if (bounces == 0 || specular_bounce) {
                L += mat.ke * throughput;  // FIXME : should be Light->Le()
            }
        }

        // Direct lighting estimation
        L += mat.kd * throughput * sample_lights(scene, wo, *mat.bsdf, pos, n, obj, hit_light);

        // Sampling new direction and accumulate indirect lighting estimation
        Sample wi = mat.bsdf->sample(wo.direction, n);
        Ray sample_ray = { .direction = wi.dir, .origin = pos + wi.dir * 0.0001 }; 

        float pdf = mat.bsdf->pdf(wo.direction, wi.dir, n);
        float f = mat.bsdf->eval_bsdf(wo.direction, wi.dir, n);
        if (f == 0.f || pdf == 0.f)
            break;
        throughput *= mat.kd * f * wi.dir.dot_product(n) / pdf;

        wo = sample_ray;
        
        float p = std::max({throughput.r, throughput.g, throughput.b});
        if (bounces > 5) {
            if (random_float(0, 1) > p) {
                break;
            }
            throughput *= 1 / (1 - p); // FIXME might be just 1/p
        } 

    }

    return L.clamp(0.f, 1.f);
}

void render_aux(Image &img, const Scene &scene, std::atomic<int>& progress, 
                size_t j_start, size_t h, size_t w) {

    size_t n_samples = 32;
    float inv_samples = 1 / (float) n_samples;
    for (size_t j = j_start; j < j_start + h; j++) {
        for (size_t i = 0; i < w; i++) {
            Ray view_ray = scene.get_camera().get_pixel_ray(i, j);

            // Path tracing
            Rgb color(0);
            for (size_t k = 0; k < n_samples; k++) {
                color += path_trace_pbr(scene, view_ray);
                //color += radiance(scene, view_ray, 0) * inv_samples;
            }

            color /= n_samples;
            
            // Whitted ray tracing
            //Rgb color = cast_ray(scene, view_ray, 0);

            img.set_pixel(i, j, color);
        }
        progress += w;
    }
}

void progress_counter(std::atomic<int>& progress, int tot) {
    while (progress != tot) {
        std::cout << (float) progress / tot * 100 << " %\n";
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

void render(Image &img, const Scene &scene) {
    int n = img.h * img.w;
    float k = 0;

    std::vector<std::thread> threads;
    size_t n_threads = std::thread::hardware_concurrency(); 
    //n_threads = 1; 
    std::atomic<int> progress(0);

    size_t block_w = img.w;
    size_t block_h = img.h / n_threads;
    size_t j_start = 0;
    for (size_t i = 0; i < n_threads; i++) {

        if (i == n_threads - 1) {
            block_h += img.h % n_threads;
        }

        threads.push_back(std::thread(
            [=, &img, &scene, &progress]{
                render_aux(img, scene, progress, j_start, block_h, block_w);
            }
        ));

        j_start += block_h;
    }

    threads.push_back(std::thread(
        [=, &progress]{
            progress_counter(progress, n);
        }
    ));

    for (auto &thread : threads) {
        thread.join();
    }

}

}