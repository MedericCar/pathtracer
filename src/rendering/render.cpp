#include <algorithm>
#include <atomic>
#include <thread>
#include <utility>

#include "../accelerator/bvh.hh"
#include "../utils/random.hh"
#include "render.hh"

namespace isim {

Rgb sample_lights(const Scene& scene, const Ray& ray_out, const Vector3& pos, 
                  const Vector3& n, const Object* target_obj,
                  const Object* hit_light, bool specular_bounce) {

  Rgb Ld(0);
  for (const auto& l : scene.get_lights()) {
    const Material* light_mat = l->get_material(pos);

    // Dismiss if already taken into account in specific cases
    if (l == hit_light) {
      continue;
    }

    float pdf = 0.0f;
    Ray ray_in = l->sample(pos, pdf);

    auto nearest_obj = scene.hit(ray_in);
    if (!nearest_obj.has_value() || nearest_obj.value().first != target_obj)
      continue;

    const Material* obj_mat = target_obj->get_material(pos);
    if (obj_mat->ks_ != Rgb(0)) {  // Not to sample light for specular
      continue;
    }

    Rgb f = obj_mat->eval_bsdf(ray_out.dir * -1, ray_in.dir * -1, n);
    if (f == Rgb(0) || pdf == 0.f)
      continue;
    
    // Sample the area : need to convert var of integration to solid angle expression
    float square_norm = (ray_in.origin - pos).euclidean_norm();
    square_norm *= square_norm;

    Vector3 light_n = l->get_normal(ray_in.origin);  // FIXME could save beforehand
    float cos1 = light_n.dot_product(ray_in.dir * -1);
    float cos2 = n.dot_product(ray_in.dir);

    Ld += light_mat->ke_ * f * cos1 * cos2 / (square_norm * pdf);
  }

  return Ld;
}


Rgb path_trace(const Scene &scene, Ray ray_out) {
  Rgb L(0.0f); 
  Rgb throughput(1.0f);
  bool specular_bounce = false;

  for (int bounces = 0; ; bounces++) {

    auto nearest_obj = scene.hit(ray_out);
    if (!nearest_obj.has_value() || bounces > MAX_DEPTH) {
      L += throughput * Rgb(0.0f);
      break;
    }

    // Recover intersection's info
    const Object* obj = nearest_obj.value().first;
    Vector3 pos = nearest_obj.value().second;
    Vector3 n = obj->get_normal(pos);
    const Material* mat = obj->get_material(pos);

    // Intersection with emissive object : two exceptions for adding
    const Object* hit_light = nullptr;
    if (mat->ke_ != Rgb(0)) {
      hit_light = obj;
      if (bounces == 0 || specular_bounce) {
        L += mat->ke_ * throughput;  // FIXME : should be Light->Le()
      }
    }

    // Direct lighting estimation
    L += throughput * sample_lights(scene, ray_out, pos, n, obj, hit_light,
                                    specular_bounce);

    // Sampling new direction and accumulate indirect lighting estimation
    Vector3 wi;
    float pdf;
    Rgb f = mat->sample_f(ray_out.dir * -1, n, &wi, &pdf);  // switch wo direction for computations
    specular_bounce = (mat->ks_ != Rgb(0));  // FIXME
    if (f == Rgb(0) || pdf == 0.f) {
      break;
    }
    throughput *= f * abs(wi.dot_product(n)) / pdf;

    if (wi.dot_product(n) < 0) {
      ray_out = { .dir = wi.normalize(), .origin = pos - n * 1e-4 };
    } else {
      ray_out = { .dir = wi.normalize(), .origin = pos + n * 1e-4 };
    }
    
    // Russian roulette to eliminate some paths
    float p = std::max({throughput.r_, throughput.g_, throughput.b_});
    if (bounces > 5) {
      if (random_float(0, 1) > p) {
        break;
      }
      throughput *= 1 / p; 
    }
  }

  return L.clamp(0.f, 1.f);
}


void render_aux(Image &img, const Scene &scene, std::atomic<int>& progress, 
                size_t j_start, size_t h, size_t w, int n_samples) {

  for (size_t j = j_start; j < j_start + h; j++) {
    for (size_t i = 0; i < w; i++) {
      Ray view_ray = scene.get_camera().get_pixel_ray(i, j);

      Rgb color(0);
      for (size_t k = 0; k < n_samples; k++) {
        color += path_trace(scene, view_ray);
      }

      color /= n_samples;
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


void render(Image &img, const Scene &scene, int n_samples) {
  int n = img.h_ * img.w_;
  float k = 0;

  std::vector<std::thread> threads;
  size_t n_threads = std::thread::hardware_concurrency(); 
  std::atomic<int> progress(0);

  size_t block_w = img.w_;
  size_t block_h = img.h_ / n_threads;
  size_t j_start = 0;
  for (size_t i = 0; i < n_threads; i++) {

    if (i == n_threads - 1) {
      block_h += img.h_ % n_threads;
    }

    threads.push_back(std::thread(
      [=, &img, &scene, &progress] {
        render_aux(img, scene, progress, j_start, block_h, block_w, n_samples);
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