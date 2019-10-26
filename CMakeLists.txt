cmake_minimum_required(VERSION 3.7)
project(vulkan_example CXX)

set(CMAKE_STANDARD 14)
add_executable(Main main.cpp)
find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})
target_link_libraries(Main ${Vulkan_LIBRARIES})
