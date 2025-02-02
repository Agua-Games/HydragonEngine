cmake_minimum_required(VERSION 3.10)

# Define the project name and version
project(HydragonEngine VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define the root directory (2 levels up from CMakeLists.txt location)
set(ENGINE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")

# Detect the platform and set the output directory
if(WIN32)
    set(PLATFORM "Win64")
    add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX -DPLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    set(PLATFORM "Linux")
    add_definitions(-DPLATFORM_LINUX)
elseif(APPLE)
    set(PLATFORM "Mac")
    add_definitions(-DPLATFORM_MAC)
else()
    set(PLATFORM "Android")
    add_definitions(-DPLATFORM_ANDROID)
endif()

# Set default build type to Release if not specified
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

# Set output directories based on platform and build type
set(OUTPUT_DIR "${ENGINE_ROOT_DIR}/Bin/${PLATFORM}/$<CONFIG>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})

# Build options
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(ENABLE_DEBUG_LOGGING "Enable debug logging" OFF)
option(ENABLE_VULKAN_VALIDATION "Enable Vulkan validation layers" ON)

# Find Vulkan package
find_package(Vulkan REQUIRED)
if(NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan SDK. Please install the Vulkan SDK and ensure it's in your PATH.")
endif()

# Collect source files automatically
file(GLOB_RECURSE SOURCES 
    "${ENGINE_ROOT_DIR}/Source/*.cpp"
    "${ENGINE_ROOT_DIR}/Source/*.h"
    "${ENGINE_ROOT_DIR}/Source/*.hpp"
)

# Group source files by directory structure
foreach(SOURCE ${SOURCES})
    get_filename_component(SOURCE_PATH "${SOURCE}" PATH)
    file(RELATIVE_PATH RELATIVE_SOURCE_PATH "${ENGINE_ROOT_DIR}/Source" "${SOURCE_PATH}")
    string(REPLACE "/" "\\" GROUP_PATH "${RELATIVE_SOURCE_PATH}")
    source_group("Source\\${GROUP_PATH}" FILES "${SOURCE}")
endforeach()

# Add executable
add_executable(Hydragon ${SOURCES})

# Include third-party libraries
add_subdirectory("${ENGINE_ROOT_DIR}/Source/ThirdParty/imgui" "${CMAKE_BINARY_DIR}/imgui")
add_subdirectory("${ENGINE_ROOT_DIR}/Source/ThirdParty/Vulkan-Hpp" "${CMAKE_BINARY_DIR}/Vulkan-Hpp")

# Link third-party libraries
target_link_libraries(Hydragon
    PRIVATE
        imgui
        Vulkan-Hpp
        Vulkan::Vulkan
)

# Include directories
target_include_directories(Hydragon
    PUBLIC
        "${ENGINE_ROOT_DIR}/Source"
    PRIVATE
        "${ENGINE_ROOT_DIR}/Source/ThirdParty/imgui"
        "${ENGINE_ROOT_DIR}/Source/ThirdParty/Vulkan-Hpp/include"
        ${Vulkan_INCLUDE_DIRS}
)

# Compiler flags and definitions
if(MSVC)
    target_compile_options(Hydragon PRIVATE 
        /W4     # Warning level 4
        /WX     # Treat warnings as errors
        /MP     # Multi-processor compilation
        /Zi     # Debug information format
    )
    target_compile_definitions(Hydragon PRIVATE
        _CRT_SECURE_NO_WARNINGS
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )
else()
    target_compile_options(Hydragon PRIVATE 
        -Wall 
        -Wextra 
        -Wpedantic 
        -Werror
    )
endif()

# Debug/Release specific settings
target_compile_definitions(Hydragon PRIVATE
    $<$<CONFIG:Debug>:_DEBUG DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

if(ENABLE_DEBUG_LOGGING)
    target_compile_definitions(Hydragon PRIVATE ENABLE_DEBUG_LOGGING=1)
endif()

if(ENABLE_VULKAN_VALIDATION)
    target_compile_definitions(Hydragon PRIVATE ENABLE_VULKAN_VALIDATION=1)
endif()

# Installation rules
include(GNUInstallDirs)
install(TARGETS Hydragon
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)

# Export compile commands for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)