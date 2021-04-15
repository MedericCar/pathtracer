#include "scene.hh"

namespace isim {

    Scene::Scene(const Camera& _camera) : camera(_camera) {}


    Scene::Scene(const Camera& _camera,
                 std::vector<std::unique_ptr<Object>> _objects,
                 std::vector<std::unique_ptr<Light>> _lights)
      : camera(_camera),
        objects(std::move(_objects)),
        lights(std::move(_lights))
    {
        root_volume = construct_tree(objects, 0, objects.size());
    }
    
    const Camera& Scene::get_camera() const {
        return camera;
    }

    const std::vector<std::unique_ptr<Object>>& Scene::get_objects() const {
        return objects;
    }
    
    const std::vector<std::unique_ptr<Light>>& Scene::get_lights() const {
        return lights;
    }

    void Scene::add_object(std::unique_ptr<Object> obj) {
        objects.push_back(std::move(obj));
    }

    void Scene::add_light(std::unique_ptr<Light> light) {
        lights.push_back(std::move(light));
    }

    void Scene::summary() {
        std::cout << "N objects : " << objects.size() << "\n";
    }

    std::optional<std::pair<Object*, Vector3>> 
    Scene::hit(const Ray& ray) const {
        return root_volume->hit(ray, 0.001, INFINITY);
    }

}

