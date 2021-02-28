#pragma once

#include <vector>

#include "object.hh"
#include "light.hh"
#include "camera.hh"

namespace isim {

    class Scene {
    
    private:
        std::vector<Object*> objects;
        std::vector<Light*> lights;
        Camera camera;

    public:
        Scene(Camera Camera);
        ~Scene();

        const Camera& get_camera() const;
        const std::vector<Object*>& get_objects() const;

        void add_object(Object* obj);
        void add_light(Light* light);

    };

}