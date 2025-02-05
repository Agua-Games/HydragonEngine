# ====================================================================================================================
# Example usage (from the repository's root directory):
#  mkdir Engine/Build/CMake/build              // Make directory for the build
#  cd Engine/Build/CMake/build                 // Change current directory to newly created build directory
#  cmake ../                // Run CMake to generate build files. Later add options, like -DCMAKE_BUILD_TYPE=Release
#  cmake --build .          // Build the project. Later add options, like --config Release
# ====================================================================================================================

# =====================================================================
# CMake version, project info
# =====================================================================
cmake_minimum_required(VERSION 3.10)
project(Hydragon VERSION 0.1.0 LANGUAGES CXX C)

# =====================================================================
# C++ standard requirements,options)
# =====================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =====================================================================
# Global options for the builds
# =====================================================================
# Optional: Default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# =====================================================================
# Detect Platform
# =====================================================================
#if(WIN32)
#    set(PLATFORM "Windows")
#elseif(UNIX AND NOT APPLE)
#    set(PLATFORM "Linux")
#elseif(APPLE)
#    set(PLATFORM "MacOS")
#endif()

# =====================================================================
# Configure paths for CMake
# =====================================================================
# Define the root directory (2 levels up from CMakeLists.txt location)
set(ENGINE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")
# Explicitly set the output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)

# Define other useful paths
set(SOURCE_DIR "${ENGINE_ROOT_DIR}/Source")
set(CORE_SOURCE_DIR "${ENGINE_ROOT_DIR}/Source/Core")
set(DEV_TOOLS_SOURCE_DIR "${ENGINE_ROOT_DIR}/Source/DevTools")
set(EDITOR_SOURCE_DIR "${ENGINE_ROOT_DIR}/Source/Editor")
set(THIRD-PARTY_SOURCE_DIR "${ENGINE_ROOT_DIR}/Source/ThirdParty")
set(BINARIES_DIR "${ENGINE_ROOT_DIR}/Bin")

# Define the output directory for the build
#set(OUTPUT_DIR "${BINARIES_DIR}/${PLATFORM}/$<CONFIG>")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

# ========================================================================================================
# Find and store external dependencies. Packages, libraries (system libraries, precompiled libraries, etc)
# =========================================================================================================
# Vulkan - Find the Vulkan SDK on the system. REQUIRED means that the build will fail if not found.
find_package(Vulkan REQUIRED)                   
set(LIBRARIES "glfw;Vulkan::Vulkan")

# ========================================================================================================
# Add subdirectories for Hydragon Dev Tools
# ========================================================================================================
# If we ever need to have a dedicated CMakeLists.txt for the DevTools, we can add it here.
# The command add_subdirectory() is used to add nested CMake projects to the build. 
# It processes the CMakeLists.txt in the specified directory.
#add_subdirectory(${SOURCE_DIR}/DevTools DevTools)          

# =================================================================================================================
# Third-party libraries: Git submodules and their dependencies - fetch, init, update
#
# - This section is used for third-party libraries that are included as git submodules (references to other repos),
#   not directly cloned into Hydragon's repository.
# - Use this section to fetch content from git submodules, to make sure they are available during build time
# =================================================================================================================
# if using submodules (and not having initialized nor updated them), fetch content
include(FetchContent)               # for more info: https://cmake.org/cmake/help/latest/module/FetchContent.html

#FetchContent_Declare(
#   [library name]
#   GIT_REPOSITORY [url]
#   GIT_TAG [tag]

#   FIND_PACKAGE_ARGS NAMES [library name] REQUIRED   # if the library is an external package
#   SOURCE_SUBDIR [CMakeLists.txtsubdirectory]    # if the CMakelists.txt is not in the root of the repo
#   URL [url]                       # use for tools, scripts, etct that won't be included in the build
#)
#FetchContent_Declare(
#   [library name]
#   GIT_REPOSITORY [url]
#   GIT_TAG [tag]
#   OVERRIDE_FIND_PACKAGE
#)
#FetchContent_MakeAvailable([lirary name] [library name] [library name] [library name]...)

# ==============================================================================================================
# Third-party libraries: GLFW, Dear ImGui, Vulkan, etc.
#
# - Store third-party directories
# - Add third-party libraries' CMake projects (their CMakeLists.txt) with add_subdirectory()
# - Set options for the third-party libraries (e.g. build examples, tests, etc)
# - Get include files automatically from the third-party directories with include_directories()
# ==============================================================================================================

# By using add_subdirectory([third-party directory]), CMake will process the CMakeLists.txt in the third-party
# directory, effectively adding the third-party library to the build process, making it available to the project.

# =====================================================================
# Third-party libraries: GLFW
# =====================================================================
set(GLFW_DIR ${THIRD-PARTY_SOURCE_DIR}/glfw)                                  # Set this to point to an up-to-date GLFW repo
add_subdirectory(${GLFW_DIR} glfw EXCLUDE_FROM_ALL)                     # Processes the CMakeLists.txt in the GLFW directory, sets a folder for the target.      
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
include_directories(${GLFW_DIR}/include)

# =====================================================================
# Third-party libraries: Dear ImGui
# =====================================================================
set(IMGUI_DIR ${THIRD-PARTY_SOURCE_DIR}/imgui)                                                    # Set this to point to an up-to-date Dear ImGui repo
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)

# Vulkan
# Currently using vulkan headers from glfw. 
# *Note: Vulkan is a system library, so it is not necessary to include it here.
# The vulkan.h header file is included in the Vulkan SDK, which should be installed on the system, available to the compiler.
# The vulkan.h file seems to be a definition of many Vulkan structures, default value mappings etc.
#include_directories(${GLFW_DIR}/deps)                       

# end of third-party libraries ---------------------------------------------------------------------------

# ==================================================================================================
# Collect source and header files automatically
#   - Only parse directories that should not be selectively included/excluded!
# =================================================================================================

file(GLOB_RECURSE SRC_FILES_AUTO
    ${CORE_SOURCE_DIR}/*.cpp
    ${DEV_TOOLS_SOURCE_DIR}/*.cpp
    ${EDITOR_SOURCE_DIR}/*.cpp)

# =====================================================================================
# Collect source files explicitly
# =====================================================================================
set(SRC_FILES_MANUAL
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/engine.cpp
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
)

# ======================================================================================
# Define executable
# ======================================================================================

# Only source files need to be added to the executable target - header files are brought
# into the source files automatically by the compiler at compile time, via #include directives.

# Define executable target, add sources to be compiled into the executable
add_executable(${PROJECT_NAME} ${SRC_FILES_AUTO} ${SRC_FILES_MANUAL})

# ======================================================================================
# Set target properties - headless/command-line application vs GUI application
#       - Hydragon can be used as a headless application (no GUI) also, for batched tasks.
# ======================================================================================

# Set the target properties for the executable
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)

# ======================================================================================
# Link libraries to the target executable
# ======================================================================================
# Some linkers require dependent libraries to be linked after the libraries they depend on.
# In this case, the order of the libraries in the target_link_libraries() command is important.
# If EngineCore depends on Vulkan::Vulkan, linking EngineCore first ensures proper resolution.

# Link the Hydragon executable to the EngineCore and EditorUI libraries.
#target_link_libraries(Hydragon PRIVATE EngineCore EditorUI)    # need to check these namings. Maybe Core and Editor.

# Enable runtime mode definitions
#target_compile_definitions(Hydragon PRIVATE ENABLE_EDITOR_SUPPORT=1)

# Add external precompiled libraries (e.g. GLFW, Vulkan, etc) to the target executable
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

# ==================================================================================
# Installation Rules
# ==================================================================================
# Install the executable to the specified directory
#install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${BINARY_DIR})

# ==================================================================================
# Optional: Custom Build Options
# ==================================================================================
option(ENABLE_DEBUG_LOGGING "Enable debug logging" OFF)
if(ENABLE_DEBUG_LOGGING)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_DEBUG_LOGGING=1)
endif()