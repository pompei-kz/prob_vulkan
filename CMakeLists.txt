cmake_minimum_required(VERSION 4.2)
project(prob_vulkan)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(SDL3 REQUIRED CONFIG)

include(cmake/DiContextGenerator.cmake)

add_executable(prob_vulkan
    src/main.cpp
    src/di/Getter.h
    src/app/Settings.cpp
    src/app/Settings.h
    src/vul/Print.cpp
    src/vul/Print.h
    src/util/util.cpp
    src/util/util.h
    src/vul/DescriptorStore.cpp
    src/vul/DescriptorStore.h
    src/vul/InitInstance.cpp
    src/vul/InitInstance.h
    src/vul/Log.cpp
    src/vul/Log.h
    src/app/MainWindow.cpp
    src/app/MainWindow.h
    src/app/FirstInit.cpp
    src/app/FirstInit.h
    src/vul/Device.cpp
    src/vul/Device.h
    src/vul/SelectPhysicalDevice.cpp
    src/vul/SelectPhysicalDevice.h
    src/vul/model/QueueFamilyIndices.cpp
    src/vul/model/QueueFamilyIndices.h
    src/vul/model/SwapChainSupport.cpp
    src/vul/model/SwapChainSupport.h
    src/vul/SwapChain.cpp
    src/vul/SwapChain.h
)

target_include_directories(prob_vulkan PRIVATE src)

generate_di_context(prob_vulkan
    SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src"
    OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated"
    GEN_GETTER_NAME "di::Getter"
    GEN_GETTER_INC "di/Getter.h"
    GEN_CONTEXT_CLASS_NAME "di::Context"
)

include(FetchContent)

FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.6.2
)

FetchContent_MakeAvailable(CLI11)

target_link_libraries(prob_vulkan PRIVATE Vulkan::Vulkan SDL3::SDL3 CLI11::CLI11)
