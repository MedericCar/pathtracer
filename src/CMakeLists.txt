set(SRC_RAYTRACER "vector.cpp"
                  "image.cpp"
                  "uniform_texture.cpp"
                  "object.cpp"
                  "sphere.cpp"
                  "camera.cpp"
                  "matrix.cpp"
                  "scene.cpp"
                  "render.cpp"
                  "point_light.cpp"
                  "color.cpp"
                  "triangle.cpp"
                  "parser.cpp")

add_library(raytracer ${SRC_RAYTRACER})

target_include_directories(raytracer PUBLIC .)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(app main.cpp)
target_link_libraries(app PRIVATE raytracer ${CONAN_LIBS})