FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.10.0
)

FetchContent_MakeAvailable(googletest)

set(SRC_TESTS "main.cpp"
              "test_vector.cpp")

add_executable(tests ${SRC_TESTS})

target_link_libraries(tests PRIVATE raytracer gtest)

add_test(NAME tests COMMAND test)