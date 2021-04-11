#include <fstream>
#include <nlohmann/json.hpp>
#include <memory>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "parser.hh"
#include "../objects/point_light.hh"
#include "../objects/sphere.hh"
#include "../objects/triangle.hh"
#include "../materials/material.hh"
#include "../bsdf/diffuse.hh"


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

        auto ka = mat.at("ka").get<std::array<float, 3>>();
        auto kd = mat.at("kd").get<std::array<float, 3>>();
        auto ks = mat.at("ks").get<std::array<float, 3>>();
        auto ke = mat.at("ke").get<std::array<float, 3>>();

        float ns = mat.at("ns").get<float>();
        float ni = mat.at("ni").get<float>();
        
        if (mat.at("type").get<std::string>() == "uniform") {
            materials.emplace_back(std::make_shared<Material>(
                Rgb(ka),  // ka
                Rgb(kd),   // kd
                Rgb(ks),  // ks
                ns,  // ns
                Rgb(ke),  // ke
                ni,  //ni
                std::make_shared<DiffuseBsdf>(DiffuseBsdf())  // bsdf
            ));
        }
    }

    return materials;
}

std::vector<std::unique_ptr<Object>> 
load_objects(json j, std::vector<std::shared_ptr<Material>> materials) {
    
    std::vector<std::unique_ptr<Object>> objects;
    
    for (auto const& obj : j.at("objects")) {                

        int i = obj.at("material").get<int>();
        std::shared_ptr<Material> mat = materials[i];
        std::string label = obj.at("label").get<std::string>();
        
        std::string type = obj.at("type").get<std::string>();
        if (type == "sphere") {
            auto c = obj.at("center").get<std::vector<float>>();
            Vector3 center(c[0], c[1], c[2]);
            float radius = obj.at("radius").get<float>();

            objects.emplace_back(std::make_unique<Sphere>(mat, label, center,
                                                          radius));
        } else if (type == "triangle") {
            auto p1 = obj.at("p1").get<std::vector<float>>();
            Vector3 v1(p1[0], p1[1], p1[2]);
            auto p2 = obj.at("p2").get<std::vector<float>>();
            Vector3 v2(p2[0], p2[1], p2[2]);
            auto p3 = obj.at("p3").get<std::vector<float>>();
            Vector3 v3(p3[0], p3[1], p3[2]);

            objects.emplace_back(std::make_unique<Triangle>(mat, label,
                                                            v1, v2, v3));
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

void load_meshes(const std::string& obj_file, const std::string& mtl_dir,
                 std::vector<std::unique_ptr<Object>>& objects) {

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> mtls;

    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &mtls, &err,
                                obj_file.c_str(), mtl_dir.c_str());

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    if (!ret) {
        exit(1);
    }

    std::vector<std::shared_ptr<Material>> materials;
    for (auto const& mtl : mtls) {
        // FIXME : follow MTL format
        materials.emplace_back(std::make_shared<Material>(
            Rgb(mtl.ambient[0], mtl.ambient[1], mtl.ambient[2]),  // ka
            Rgb(mtl.diffuse[0], mtl.diffuse[1], mtl.diffuse[2]),  // kd
            Rgb(mtl.specular[0], mtl.specular[1], mtl.specular[2]),  // ks
            mtl.shininess,  // ns
            Rgb(mtl.emission[0], mtl.emission[1], mtl.emission[2]),  // ke
            mtl.ior,  // ni
            std::make_shared<DiffuseBsdf>(DiffuseBsdf()) // bsdf
        ));
    }

    // Traverse shapes
    for (size_t s = 0; s < shapes.size(); s++) {

        // Find mesh material
        int mat_id = shapes[s].mesh.material_ids[0];
        std::shared_ptr<Material> mat = materials[mat_id];

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
                auto triangle = 
                    std::make_unique<Triangle>(mat, "triangle",
                                               vec[0], vec[1], vec[2]);
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

    // Load meshes and their materials from OBJ & MTL
    auto obj_files = j.at("objFiles").get<std::vector<std::string>>();
    auto mtl_dir = j.at("mtlDir").get<std::string>();
    for (auto const& f : obj_files) {
        load_meshes(f, mtl_dir, objects);
    }

    return new Scene(cam, std::move(objects), std::move(lights));
}

}
