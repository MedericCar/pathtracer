set(SRC_RAYTRACER "vector.cpp"
                  "image.cpp")

add_library(raytracer ${SRC_RAYTRACER})

target_include_directories(raytracer PUBLIC .)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE raytracer)