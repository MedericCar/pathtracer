#include <utility>
#include <algorithm>
#include <thread>
#include <atomic>

#include "../utils/random.hh"
#include "render.hh"

namespace isim {

std::optional<std::pair<Object const*, Vector3>> 
nearest_intersection(const std::vector<std::unique_ptr<Object>>& objects,
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
    auto nearest_obj = nearest_intersection(scene.get_objects(), ray);

    // Set background
    if (!nearest_obj.has_value() && depth <= 1)
        return Rgb(0.66, 0.79, 1);
        //return color;

    // Recursion stopping case
    if (!nearest_obj.has_value() || depth == MAX_DEPTH)
        return color;

    const Object* obj = nearest_obj.value().first;
    Vector3 pos = nearest_obj.value().second;
    Material mat = obj->get_material(pos);

    Vector3 n = obj->get_normal(pos);

    for (auto const& p : scene.get_lights()) {
        Ray light_ray = p->get_ray(pos);

        // Shadows
        auto light_inter = nearest_intersection(scene.get_objects(), 
                                                     light_ray);
        if (!light_inter || light_inter.value().first != obj)
            continue;

        // Ambient component
        color = mat.constants.ka * mat.constants.ke;

        // Diffuse component
        Vector3 l = light_ray.dir * -1;
        float l_dot_n = l.dot_product(n);
        color += mat.constants.kd * l_dot_n; 

        // Specular component
        Vector3 r = n * 2 * l_dot_n - l;
        float v_dot_r = (ray.dir * -1).dot_product(r);
        color += mat.constants.ks 
            * std::max(0.0, pow(v_dot_r, (int) mat.constants.ns));
    }

    // Reflection
    Ray reflect_ray = Ray{
        .dir = ray.dir - n * 2 * ray.dir.dot_product(n),
        .origin = pos + n * 0.001
    };

    float k = (color.r + color.g + color.b) / 3;
    color += cast_ray(scene, reflect_ray, depth + 1) * mat.constants.ks * k;

    return color;
}

Rgb radiance(const Scene &scene, const Ray& ray, int depth) {
    Rgb color = Rgb(0.0f); 
    auto nearest_obj = nearest_intersection(scene.get_objects(), ray);

    // Recursion stopping case
    if (!nearest_obj.has_value())
        return color;

    const Object* obj = nearest_obj.value().first;
    Vector3 pos = nearest_obj.value().second;
    Material mat = obj->get_material(pos);
    Vector3 n = obj->get_normal(pos);

    // Add direct light
    Rgb direct = mat.constants.ke;
    if (depth == MAX_DEPTH || direct != Rgb(0)) {
        return direct;
    }

    // Add indirect light by sampling
    Rgb indirect = Rgb(0);
    Vector3 wi = mat.bsdf->sample(ray.dir, n);
    Ray sample_ray = { .dir = wi, .origin = pos + wi * 0.001 }; 

    float pdf = mat.bsdf->pdf(ray.dir, wi, n);
    Rgb bsdf = mat.bsdf->eval_bsdf(ray.dir, wi, n);
    indirect += radiance(scene, sample_ray, depth + 1) * bsdf / pdf;

    color = direct + indirect;

    return color;
}

Rgb sample_lights(const Scene& scene, const Ray& ray_out, const Vector3& pos, 
                  const Vector3& n, const Object* target_obj,
                  const Object* hit_light) {

    Rgb Ld(0);
    for (auto const& ptr : scene.get_objects()) {  // BIG FIXME IF THERE ARE MANY OBJECTS RIP
        const Object* l = ptr.get();
        Material light_mat = l->get_material(pos);

        // Dismiss if not light or taken into account in specific cases
        if (light_mat.constants.ke == Rgb(0) || l == hit_light) {
            continue;
        }

        float pdf = 0.0f;
        Ray ray_in = l->sample(pos, pdf);

        auto nearest_obj = nearest_intersection(scene.get_objects(), ray_in);
        if (!nearest_obj.has_value() || nearest_obj.value().first != target_obj)
            continue;

        Rgb f = target_obj->get_material(pos).eval_bsdf(ray_out.dir,
                                                        ray_in.dir * -1, n);
        if (f == Rgb(0) || pdf == 0.f)
            continue;
        
        // Sample the area : need to convert var of integration to solid angle expression
        float square_norm = (ray_in.origin - pos).euclidean_norm();
        square_norm *= square_norm;

        Vector3 light_n = l->get_normal(ray_in.origin);  // FIXME could save beforehand
        float cos1 = light_n.dot_product(ray_in.dir);
        float cos2 = n.dot_product(ray_in.dir * -1);

        Ld += f * cos1 * cos2 * 2 / (square_norm * pdf);
    }

    return Ld;
}

Rgb path_trace_pbr(const Scene &scene, Ray ray_out) {
    Rgb L(0.0f); 
    Rgb throughput(1.0f);
    bool specular_bounce = false;

    for (int bounces = 0; ; bounces++) {

        auto nearest_obj = nearest_intersection(scene.get_objects(), ray_out);
        if (!nearest_obj.has_value() || bounces > MAX_DEPTH) {
            L += throughput * Rgb(0);
            break;
        }

        // Recover intersection's info
        const Object* obj = nearest_obj.value().first;
        Vector3 pos = nearest_obj.value().second;
        Vector3 n = obj->get_normal(pos);
        Material mat = obj->get_material(pos);

        // Intersection with emissive object : two exceptions for adding
        const Object* hit_light = nullptr;
        if (mat.constants.ke != Rgb(0)) {
            hit_light = obj;
            if (bounces == 0 || specular_bounce) {
                L += mat.constants.ke * throughput;  // FIXME : should be Light->Le()
            }
        }

        // Direct lighting estimation
        L += throughput * sample_lights(scene, ray_out, pos, n, obj, hit_light);

        // Sampling new direction and accumulate indirect lighting estimation
        Vector3 wi = mat.bsdf->sample(ray_out.dir, n);
        Ray sample_ray = { .dir = wi, .origin = pos + wi * 0.0001 }; 

        float pdf = mat.bsdf->pdf(ray_out.dir, wi, n);
        Rgb f = mat.eval_bsdf(ray_out.dir, wi, n);
        if (f == 0.f || pdf == 0.f)
            break;
        throughput *= f * wi.dot_product(n) / pdf;

        ray_out = sample_ray;
        
        // Russian roulette to eliminate some paths
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
                //color += radiance(scene, view_ray, 0);
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