cmake_minimum_required(VERSION 3.16.3)

project(Raytracer VERSION 1.0
                  DESCRIPTION "Raytracer for the first TP of ISIM"
                  LANGUAGE CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(raytracer "src/main.cpp")

# PUBLIC : any target that links to this target must also need the include dir.