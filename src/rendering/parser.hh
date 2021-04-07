// pipeline :
//  - read json <- load each component
//  - one function per component
//  - return loaded scene

#include "scene.hh"

namespace isim {
    
    Scene* load_scene(const std::string& filename);  

}
