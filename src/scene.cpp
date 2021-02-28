#include "scene.hh"

namespace isim {

    Scene::Scene(Camera _camera) : camera(_camera) {}

    Scene::~Scene() {
        for (auto p : objects)
            delete p;

        for (auto p : lights)
            delete p;
    }
    
    const Camera& Scene::get_camera() const {
        return camera;
    }

    const std::vector<Object*>& Scene::get_objects() const {
        return objects;
    }

    void Scene::add_object(Object* obj) {
        objects.push_back(obj);
    }

    void Scene::add_light(Light* light) {
        lights.push_back(light);
    }

}

