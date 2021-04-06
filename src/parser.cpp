#include <fstream>
#include <nlohmann/json.hpp>

#include "parser.hh"
#include "point_light.hh"
#include "sphere.hh"
#include "uniform_texture.hh"

using json = nlohmann::json;

namespace isim {

Camera load_camera(json j) {
    
    j = j.at("camera");

    auto c = j.at("center").get<std::vector<float>>();
    Vector3 center(c[0], c[1], c[2]);

    auto t = j.at("target").get<std::vector<float>>();
    Vector3 target(t[0], t[1], t[2]);

    float fov = j.at("fov").get<float>();
    int width = j.at("width").get<int>();
    int height = j.at("height").get<int>();

    return Camera(center, target, fov, width, height);
}

std::vector<std::shared_ptr<Material>> load_materials(json j) {
    
    std::vector<std::shared_ptr<Material>> materials;
    
    for (auto const& mat : j.at("materials")) {                

        auto c = mat.at("color").get<std::vector<float>>();
        Rgb color(255 * c[0], 255 * c[1], 255 * c[2]);

        float kd = mat.at("kd").get<float>();
        float kr = mat.at("kr").get<float>();
        float ks = mat.at("ks").get<float>();
        
        if (mat.at("type").get<std::string>() == "uniform") {
            MaterialConstants c = { color, kd, kr, ks };
            materials.emplace_back(std::make_shared<UniformTexture>(c));
        }
    }

    return materials;
}

std::vector<std::unique_ptr<Object>> 
load_objects(json j, std::vector<std::shared_ptr<Material>> materials) {
    
    std::vector<std::unique_ptr<Object>> objects;
    
    for (auto const& obj : j.at("objects")) {                
        
        if (obj.at("type").get<std::string>() == "sphere") {
            auto c = obj.at("center").get<std::vector<float>>();
            Vector3 center(c[0], c[1], c[2]);

            std::shared_ptr<Material> mat = materials[obj.at("material").get<int>()];
            std::string label = obj.at("label").get<std::string>();
            float radius = obj.at("radius").get<float>();
            objects.emplace_back(std::make_unique<Sphere>(mat, label, center, radius));
        }
    }

    return objects;
}

std::vector<std::unique_ptr<Light>> load_lights(json j) {

    std::vector<std::unique_ptr<Light>> lights;
    
    for (auto const& li : j.at("lights")) {                
        
        if (li.at("type").get<std::string>() == "point") {
            auto p = li.at("pos").get<std::vector<float>>();
            Vector3 pos(p[0], p[1], p[2]);
            lights.emplace_back(std::make_unique<PointLight>(pos));
        }
    }

    return lights;
}

Scene* load_scene(const std::string& filename) {

    std::ifstream file(filename);
    json j = json::parse(file);

    Camera cam = load_camera(j);
    std::vector<std::shared_ptr<Material>> materials = load_materials(j);
    std::vector<std::unique_ptr<Object>> objects = load_objects(j, materials); 
    std::vector<std::unique_ptr<Light>> lights = load_lights(j); 
    
    return new Scene(cam, std::move(objects), std::move(lights));
}

}
