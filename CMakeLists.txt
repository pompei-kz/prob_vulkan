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
    src/vul/HandleStore.cpp
    src/vul/HandleStore.h
    src/vul/InitInstance.cpp
    src/vul/InitInstance.h
    src/util/Log.cpp
    src/util/Log.h
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
    src/cmd/Cmd.h
    src/cmd/Cmd.cpp
    src/cmd/CmdLog.h
    src/cmd/ExecuteCmd.cpp
    src/cmd/ExecuteCmd.h
    src/back/BackStart.cpp
    src/back/BackStart.h
    src/app/Application.cpp
    src/app/Application.h
    src/cmd/CmdSequence.h
    src/cmd/CmdChangeCamera.h
    src/cmd/model/Planes.h
    src/cmd/CmdPipeline.h
    src/vul/pipeline/Pipeline.cpp
    src/vul/pipeline/Pipeline.h
    src/vul/pipeline/Pipeline_ShapeGroup.cpp
    src/vul/pipeline/Pipeline_ShapeGroup.h
    src/vul/pipeline/Pipelines.cpp
    src/vul/pipeline/Pipelines.h
    src/vul/pipeline/Pipeline_ShapesGroup_Worker.cpp
    src/vul/pipeline/Pipeline_ShapesGroup_Worker.h
    src/vul/SwapChain_Worker.cpp
    src/vul/SwapChain_Worker.h
    src/vul/Device_Worker.cpp
    src/vul/Device_Worker.h
    src/vul/model/Queues.cpp
    src/vul/model/Queues.h
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

target_sources(prob_vulkan
    PRIVATE
    FILE_SET CXX_MODULES FILES
    src/util/SyncQueue.ixx
)

target_compile_options(prob_vulkan PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wno-potentially-evaluated-expression>
)
