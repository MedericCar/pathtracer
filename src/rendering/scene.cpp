#include "scene.hh"

namespace isim {

Scene::Scene(const Camera& camera) : camera_(camera) {}


Scene::Scene(const Camera& camera, std::vector<std::unique_ptr<Object>> objects,
             std::vector<const Object*> lights)
    : camera_(camera),
      objects_(std::move(objects)),
      lights_(lights) {

  root_volume_ = construct_tree(objects_, 0, objects_.size());
}


const Camera& Scene::get_camera() const {
  return camera_;
}


const std::vector<std::unique_ptr<Object>>& Scene::get_objects() const {
  return objects_;
}


const std::vector<const Object*>& Scene::get_lights() const {
  return lights_;
}


void Scene::add_object(std::unique_ptr<Object> obj) {
  objects_.push_back(std::move(obj));
}


void Scene::add_light(const Object* light) {
  lights_.push_back(light);
}


void Scene::summary() {
  std::cout << "N objects : " << objects_.size() << "\n";
}


std::optional<std::pair<Object*, Vector3>> 
Scene::hit(const Ray& ray) const {
  return root_volume_->hit(ray, 0.001, INFINITY);
}

}

