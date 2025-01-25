cmake_minimum_required(VERSION 3.10)
project(HydragonEngine)

# Set paths
set(ENGINE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/../../Source")
set(THIRDPARTY_SOURCE_DIR "${ENGINE_SOURCE_DIR}/ThirdParty")
set(BIN_DIR "${CMAKE_SOURCE_DIR}/../../Bin")  # Updated variable name and path
set(BUILD_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/../../BuildOutput")

# Configure build output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}/Hydragon)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR}/Hydragon)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR}/Hydragon)

# Include third-party libraries - source files
add_subdirectory(${THIRDPARTY_SOURCE_DIR}/glfw ${BIN_DIR}/ThirdParty/glfw)
add_subdirectory(${THIRDPARTY_SOURCE_DIR}/imgui ${BIN_DIR}/ThirdParty/imgui)

# Include Vulkan SDK headers
include_directories($ENV{VULKAN_SDK}/Include)

# Include Vulkan-Hpp headers
include_directories(${THIRDPARTY_SOURCE_DIR}/Vulkan-Hpp/include)

# Include engine source files
file(GLOB_RECURSE ENGINE_SOURCES "${ENGINE_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "${ENGINE_SOURCE_DIR}/*.h")

# Create the engine executable
add_executable(Hydragon ${ENGINE_SOURCES} ${ENGINE_HEADERS})
target_include_directories(Hydragon PUBLIC ${ENGINE_SOURCE_DIR})
target_link_libraries(Hydragon glfw imgui)