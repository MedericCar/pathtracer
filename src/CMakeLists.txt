set(SRC_RAYTRACER "accelerator/bvh.cpp"
                  "materials/diffuse.cpp"
                  "materials/material.cpp"
                  "materials/microfacet.cpp"
                  "materials/specular.cpp"
                  "objects/object.cpp"
                  "objects/point_light.cpp"
                  "objects/sphere.cpp"
                  "objects/triangle.cpp"
                  "rendering/camera.cpp"
                  "rendering/image.cpp"
                  "rendering/parser.cpp"
                  "rendering/render.cpp"
                  "rendering/scene.cpp"
                  "utils/color.cpp"
                  "utils/matrix.cpp"
                  "utils/random.cpp"
                  "utils/vector.cpp")

add_library(raytracer ${SRC_RAYTRACER})

target_include_directories(raytracer PUBLIC "accelerator"
                                            "bsdf"
                                            "materials"
                                            "objects"
                                            "rendering"
                                            "utils")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Threads)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE raytracer ${CONAN_LIBS} ${CMAKE_THREAD_LIBS_INIT})