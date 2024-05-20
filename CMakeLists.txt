cmake_minimum_required(VERSION 3.4...3.18)
project(cmakes)

add_subdirectory(pybind11)
pybind11_add_module(cmakes src/main.cpp)

add_subdirectory(matplotplusplus)
add_subdirectory(AudioFile)



target_link_libraries(cmakes PUBLIC matplot)
target_link_libraries(cmakes PRIVATE AudioFile)





# EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(cmakes
                           PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
