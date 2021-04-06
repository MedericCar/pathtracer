#include <fstream>
#include <nlohmann/json.hpp>
#include <memory>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "parser.hh"
#include "point_light.hh"
#include "sphere.hh"
#include "triangle.hh"
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

            int i = obj.at("material").get<int>();
            std::shared_ptr<Material> mat = materials[i];
            std::string label = obj.at("label").get<std::string>();
            float radius = obj.at("radius").get<float>();

            objects.emplace_back(std::make_unique<Sphere>(mat, label, center,
                                                          radius));
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

void load_OBJ(json j, std::vector<std::unique_ptr<Object>>& objects) {

    std::string inputfile = "/home/jenntedra/Documents/epita/image/s8/isim/isim-raytracer/scenes/cornell.obj";
    std::string mlt_dir = "/home/jenntedra/Documents/epita/image/s8/isim/isim-raytracer/scenes/";
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str(), mlt_dir.c_str());

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    if (!ret) {
        exit(1);
    }

    // Traverse shapes
    for (size_t s = 0; s < shapes.size(); s++) {

        // Traverse all faces in shape
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {

            // Traverse all vertices in face
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
            std::vector<Vector3> vec;
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                float vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                float vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                float vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
                vec.push_back(Vector3(vx, vy, vz));
            }

            if (fv == 3) {
                // FIXME NOOOOOOOOOOOOOOOOOOOOOoo
                MaterialConstants c = { .color = Rgb(255, 0, 0), .kd = 1, .kr = 1, .ks = 0.7 };
                std::shared_ptr<Material> mat_triangle = std::make_shared<UniformTexture>(c);
                auto triangle = std::make_unique<Triangle>(mat_triangle,
                                                           "t",
                                                           vec[0],
                                                           vec[1],
                                                           vec[2]);
                objects.push_back(std::move(triangle));
            } else {
                std::cerr << "Houston we got a mesh problem.\n";
            }

            index_offset += fv;
        }
    }
}

Scene* load_scene(const std::string& filename) {

    std::ifstream file(filename);
    json j = json::parse(file);

    // Load user defined components
    Camera cam = load_camera(j);
    std::vector<std::shared_ptr<Material>> materials = load_materials(j);
    std::vector<std::unique_ptr<Object>> objects = load_objects(j, materials); 
    std::vector<std::unique_ptr<Light>> lights = load_lights(j); 

    // Load OBJ alongside their MTL
    //load_MTL(j, materials);
    load_OBJ(j, objects);

    return new Scene(cam, std::move(objects), std::move(lights));
}

}
