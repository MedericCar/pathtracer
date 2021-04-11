cmake_minimum_required(VERSION 3.16.3)

project(Raytracer LANGUAGES CXX
                  VERSION 1.0
                  DESCRIPTION "Raytracer for the first TP of ISIM")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -pg")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
   
include(FetchContent)

# Test management
option(ENABLE_UNIT_TESTS "Enable unit tests" ON)
message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")

if (${ENABLE_UNIT_TESTS}) 
    enable_testing()
    add_subdirectory(tests)
endif()

# The compiled library and executable is here
add_subdirectory(src)
