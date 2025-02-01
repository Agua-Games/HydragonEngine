cmake_minimum_required(VERSION 3.10)
project(HydragonEngine)

# Detect the platform 
if(WIN32)
    set(PLATFORM "win64")
elseif(UNIX AND NOT APPLE)
    set(PLATFORM "linux")
elseif(APPLE)
    set(PLATFORM "macos")
endif()

# Set paths 
set(ENGINE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/../../Source")
set(THIRDPARTY_SOURCE_DIR "${ENGINE_SOURCE_DIR}/ThirdParty")
set(BIN_DIR "${CMAKE_SOURCE_DIR}/../../Bin")
set(BUILD_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/../../BuildOutput")

# Set output directory based on build configuration (Debug/Release)
set(OUTPUT_DIR ${BIN_DIR}/${PLATFORM}/$<CONFIG>)

# Set output directories for executables, shared libraries, and static libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})

# Include FetchContent
include(FetchContent)

# Disable treating warnings as errors in SPIRV-Tools
set(SPIRV_WARN_EVERYTHING OFF CACHE BOOL "Disable all warnings in SPIRV-Tools")
set(SPIRV_WERROR OFF CACHE BOOL "Disable treating warnings as errors in SPIRV-Tools")

# Declare and fetch SPIRV-Headers
FetchContent_Declare(
  SPIRV-Headers
  GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Headers.git
  GIT_TAG vulkan-sdk-1.4.304.0
)
# Set the path to SPIRV-Headers for SPIRV-Tools
set(SPIRV-Headers_SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/spirv-headers-src)

# Make SPIRV-Headers available
FetchContent_MakeAvailable(SPIRV-Headers)

# Declare and fetch SPIRV-Tools (dependency of glslang)
FetchContent_Declare(
  SPIRV-Tools
  GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Tools.git
  GIT_TAG v2024.2
)
# Make SPIRV-Tools available
FetchContent_MakeAvailable(SPIRV-Tools)

# Set the path to SPIRV-Tools for glslang
set(SPIRV-Tools_SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/spirv-tools-src)

# Enable ENABLE_OPT in glslang
set(ENABLE_OPT ON CACHE BOOL "Enable SPIRV-Tools optimizations in glslang")

# Declare and fetch glslang (dependency of Vulkan-Hpp)
FetchContent_Declare(
  glslang
  GIT_REPOSITORY https://github.com/KhronosGroup/glslang.git
  GIT_TAG 15.1.0
)
# Make glslang available
FetchContent_MakeAvailable(glslang)

#Declare and fetch tinyxml2 (dependency of Vulkan-Hpp)
FetchContent_Declare(
  tinyxml2
  GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
  GIT_TAG 10.0.0
)
# Make tinyxml2 available
FetchContent_MakeAvailable(tinyxml2)

# Ensure glslang and SPIRV-Tools are built as shared libraries
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries")

# Make the fetched content available
FetchContent_MakeAvailable(
    SPIRV-Headers 
    SPIRV-Tools 
    glslang 
    tinyxml2)

# Set the binary directory for third-party libraries
set(THIRDPARTY_BINARY_DIR ${CMAKE_BINARY_DIR}/ThirdParty)

# Include third-party libraries
add_subdirectory(${THIRDPARTY_SOURCE_DIR}/glfw ${THIRDPARTY_BINARY_DIR}/glfw)
add_subdirectory(${THIRDPARTY_SOURCE_DIR}/imgui ${THIRDPARTY_BINARY_DIR}/imgui)
add_subdirectory(${THIRDPARTY_SOURCE_DIR}/Vulkan-Hpp ${THIRDPARTY_BINARY_DIR}/Vulkan-Hpp)
add_subdirectory(${THIRDPARTY_SOURCE_DIR}/glm ${THIRDPARTY_BINARY_DIR}/glm)

# Include headers for third-party libraries
target_include_directories(Hydragon PUBLIC
    ${THIRDPARTY_SOURCE_DIR}/glfw}/include
    ${THIRDPARTY_SOURCE_DIR}/imgui}
    ${THIRDPARTY_SOURCE_DIR}/Vulkan-Hpp}/include
    $ENV{VULKAN_SDK}/include
)

# Add engine source files
file(GLOB_RECURSE ENGINE_SOURCES "${ENGINE_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "${ENGINE_SOURCE_DIR}/*.h")

# Create the engine executable
add_executable(Hydragon ${ENGINE_SOURCES} ${ENGINE_HEADERS})

# Link third-party libraries
target_link_libraries(Hydragon
    glfw
    imgui
    Vulkan-Hpp
    glm
    SPIRV-Tools
    glslang
    tinyxml2
)

# Copy third-party binaries to the engine's output directory
add_custom_command(TARGET Hydragon POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:glfw>
        $<TARGET_FILE:imgui>
        $<TARGET_FILE:glslang>
        $<TARGET_FILE:SPIRV-Tools>
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

#debug message - print the path to the engine source files
message(STATUS "Engine source directory: ${ENGINE_SOURCE_DIR}")