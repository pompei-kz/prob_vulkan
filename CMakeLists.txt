cmake_minimum_required(VERSION 4.2)
project(prob_vulkan)

set(CMAKE_CXX_STANDARD 26)

find_package(Vulkan REQUIRED)
find_package(SDL3 REQUIRED CONFIG)

add_executable(prob_vulkan
    src/main.cpp
    src/prints.cpp
    src/app/AppInitializer.cpp
    src/app/AppInitializer.h
    src/di/Getter.h
    src/app/Settings.cpp
    src/app/Settings.h
    src/vul/Print.cpp
    src/vul/Print.h)

target_link_libraries(prob_vulkan PRIVATE Vulkan::Vulkan SDL3::SDL3)

include_directories(
    src
)
