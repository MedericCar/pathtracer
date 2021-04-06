// pipeline :
//  - read json <- load each component
//  - one function per component
//  - return loaded scene

#include <fstream>
#include <nlohmann/json.hpp>

#include "parser.hh"

using json = nlohmann::json;

namespace isim {

    //Camera load_camera(json j) {
    //    return Camera;
    //}

    //std::vector<std::unique_ptr<Object>> load_objects(json j) {
    //    return;
    //}

    //std::vector<std::unique_ptr<Light>> load_lights(json j) {
    //    return;
    //}

    //Scene* load_scene(const std::string& filename) {

    //    std::ifstream file(filename);
    //    json j = json::parse(file);

    //    Camera cam = load_camera(j);
    //    std::vector<std::unique_ptr<Object>> objects = load_objects(j); 
    //    std::vector<std::unique_ptr<Light>> lights = load_lights(j); 
    //    
    //    return new Scene(cam, objects, lights);
    //}

}
