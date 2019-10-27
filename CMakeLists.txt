cmake_minimum_required(VERSION 3.7)
project(vulkan_example CXX)

set(CMAKE_STANDARD 14)
add_executable(Main main.cpp)
find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})
target_link_libraries(Main ${Vulkan_LIBRARIES} glfw)

add_executable(ParamPack param_pack.cpp)

add_executable(Vector vector_manipulations.cpp)
