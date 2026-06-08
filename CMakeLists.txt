cmake_minimum_required(VERSION 4.2)
project(prob_vulkan)

set(CMAKE_CXX_STANDARD 26)

find_package(Vulkan REQUIRED)
find_package(SDL3 REQUIRED CONFIG)

add_executable(prob_vulkan src/main.cpp)

target_link_libraries(prob_vulkan PRIVATE Vulkan::Vulkan SDL3::SDL3)
