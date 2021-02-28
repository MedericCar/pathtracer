set(SRC_RAYTRACER "vector.cpp"
                  "image.cpp"
                  "uniform_texture.cpp"
                  "object.cpp"
                  "sphere.cpp"
                  "camera.cpp"
                  "matrix.cpp")

add_library(raytracer ${SRC_RAYTRACER})

target_include_directories(raytracer PUBLIC .)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE raytracer)