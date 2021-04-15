#pragma once

#include <vector>

#include "../objects/object.hh"
#include "../objects/light.hh"
#include "../accelerator/bvh.hh"
#include "camera.hh"

namespace isim {

    class Scene {
    
    private:
        std::vector<std::unique_ptr<Object>> objects;
        std::vector<std::unique_ptr<Light>> lights;
        BVHNode* root_volume;
        Camera camera;

    public:
        Scene(const Camera& camera);
        Scene(const Camera& camera,
              std::vector<std::unique_ptr<Object>> objects,
              std::vector<std::unique_ptr<Light>> lights);

        const Camera& get_camera() const;
        const std::vector<std::unique_ptr<Object>>& get_objects() const;
        const std::vector<std::unique_ptr<Light>>& get_lights() const;

        void add_object(std::unique_ptr<Object> obj);
        void add_light(std::unique_ptr<Light> light);

        void summary();

        std::optional<std::pair<Object*, Vector3>> hit(const Ray& ray) const;

    };

}