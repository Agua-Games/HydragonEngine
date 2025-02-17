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
# C++ standard requirements,options
# =====================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =====================================================================
# Volk requirements, options
# =====================================================================
#set(VOLK_STATIC_LOADING ON)

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
if(WIN32)
    set(PLATFORM "Windows")
elseif(UNIX AND NOT APPLE)
    set(PLATFORM "Linux")
elseif(APPLE)
    set(PLATFORM "MacOS")
endif()

# =====================================================================
# Configure paths for CMake
# =====================================================================
# PROJECT/REPO ROOT - Define the root directory (2 levels up from CMakeLists.txt location)
set(ENGINE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")

# Define other useful paths
set(SOURCE_DIR "${ENGINE_ROOT_DIR}/Source")
set(CORE_SOURCE_DIR "${ENGINE_ROOT_DIR}/Source/Core")
set(DEV_TOOLS_SOURCE_DIR "${ENGINE_ROOT_DIR}/Source/DevTools")
set(EDITOR_SOURCE_DIR "${ENGINE_ROOT_DIR}/Source/Editor")
set(THIRD-PARTY_SOURCE_DIR "${ENGINE_ROOT_DIR}/Source/ThirdParty")
set(BINARIES_DIR "${ENGINE_ROOT_DIR}/Bin")

# Define Volk's directory
# volk is a meta-loader for Vulkan. It allows you to dynamically load entrypoints required to use Vulkan.
set(VOLK_DIR "${THIRD-PARTY_SOURCE_DIR}/volk")

# BINARIES - Define the output directory for the builds -----------------
set(OUTPUT_DIR "${BINARIES_DIR}/${PLATFORM}/$<CONFIG>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})           # .exe, .so
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})           # .lib, .a
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})           # .dll, .so

# INSTALL - Define CMAKE_INSTALL_PREFIX with Custom Engine Path ---------
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${ENGINE_ROOT_DIR}/Engine/Install" CACHE PATH "Install directory" FORCE)
endif()

# Define output directories for different platforms/configurations.
# (Why use CMAKE_SYSTEM_NAME instead of PLATFORM? Because CMAKE_SYSTEM_NAME is a CMake variable that 
# is set by CMake to the name of the host system, automatically - so better suited for end user installs.)
set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/Bin/${CMAKE_SYSTEM_NAME}/$<CONFIG>")
set(INSTALL_ASSETS_DIR "${CMAKE_INSTALL_PREFIX}/Assets")
set(INSTALL_CONFIG_DIR "${CMAKE_INSTALL_PREFIX}/Config")

# ========================================================================================================
# Find and store external dependencies. Packages, libraries (system libraries, precompiled libraries, etc)
# =========================================================================================================
# Vulkan - Find the Vulkan SDK on the system. REQUIRED means that the build will fail if not found.
find_package(Vulkan REQUIRED) 

# fmt - Find the fmt library on the system. REQUIRED means that the build will fail if not found.
# fmt is a modern formatting library for C++. It provides a safe alternative to printf and sprintf.

set(LIBRARIES 
    glfw 
    Vulkan::Vulkan
    fmt::fmt)

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

#FetchContent_MakeAvailable([library name] [library name] [library name] [library name]...)

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
#include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)
include_directories(${IMGUI_DIR})

# =====================================================================
# Third-party libraries: Vulkan
# =====================================================================
# We were initially using Vulkan through the header in glfw/glad, but it is better to use the Vulkan SDK,
# which should be installed on the system, available to the compiler. It can be downloaded at 
# https://vulkan.lunarg.com/sdk/home.
# The vulkan.h file seems to be a definition of many Vulkan structures, default value mappings etc. 

# =====================================================================
# Third-party libraries: Volk
# =====================================================================
# Volk - Vulkan meta-loader.
# Currently, we are using Volk for Vulkan loading and function resolution. We're using it as a
# git submodule and building it as a static library, which is the recommended way to use it.
add_subdirectory(${VOLK_DIR} volk EXCLUDE_FROM_ALL)

# =====================================================================
# Third-party libraries: fmt
# =====================================================================
# fmt is a modern formatting library for C++. It provides a safe alternative to printf and sprintf.
add_subdirectory(${SOURCE_DIR}/ThirdParty/fmt fmt EXCLUDE_FROM_ALL)

# =====================================================================
# IconFontCppHeaders
# =====================================================================
include_directories(${SOURCE_DIR}/ThirdParty/IconFontCppHeaders)

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
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    #${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp    # removed in favor of our own Core/Graphics/Vulkan/VulkanBackend.cpp
)

# ======================================================================================
# Define executable
# ======================================================================================

# Only source files need to be added to the executable target - header files are brought
# into the source files automatically by the compiler at compile time, via #include directives.

# Define executable target, add sources to be compiled into the executable
add_executable(${PROJECT_NAME} 
    ${SRC_FILES_AUTO} 
    ${SRC_FILES_MANUAL}
)

# ======================================================================================
# Set target properties - headless/command-line application vs GUI application
#       - Hydragon can be used as a headless application (no GUI) also, for batched tasks.
# ======================================================================================

# Set the target properties for the executable
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)

# ======================================================================================
# Include directories from external dependencies to target executable
# ======================================================================================

# vulkan/vulkan.h: This is the Vulkan header file, which provides the function declarations 
# and types for Vulkan. It is included in the source code to access Vulkan’s API.
#
# vulkan-1.lib: This is the Vulkan import library, which is used during linking to resolve 
# Vulkan functions at runtime. It is required to link the application with the Vulkan loader.
# So, here we include the Vulkan include directory and link the Vulkan library.
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${Vulkan_INCLUDE_DIRS}
    ${VULKAN_SDK}/Include)

# ======================================================================================
# Include directories from third-party libraries to target executable
# ======================================================================================
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${IMGUI_DIR}/backends)

# Explicitly add the imgui backend headers you want to include, so that we won't include backends
# we're replacing, like imgui_impl_vulkan.h
target_sources(${PROJECT_NAME} PRIVATE
    ${IMGUI_DIR}/backends/imgui_impl_glfw.h
    #${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp    # removed in favor of our own Core/Graphics/Vulkan/VulkanBackend
)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${THIRD-PARTY_SOURCE_DIR}/IconFontCppHeaders)

# ======================================================================================
# Include engine directories (Core, Devtools, Editor) to target executable
# ======================================================================================

# Option 1: Manually list subdirectories
#target_include_directories(${PROJECT_NAME} PUBLIC
#    ${CORE_SOURCE_DIR}
#    ${CORE_SOURCE_DIR}/
#    ${CORE_SOURCE_DIR}/Rendering
#    ${CORE_SOURCE_DIR}/Utils
#)

# Option 2: Automatically collect subdirectories (if needed). 

# Firs implement a recursive agnostic loop function:
function(get_all_subdirectories root_dir result)
    file(GLOB_RECURSE subdirs LIST_DIRECTORIES true "${root_dir}/*")
    set(dir_list "")
    foreach(subdir ${subdirs})
        if(IS_DIRECTORY ${subdir})
            list(APPEND dir_list ${subdir})
        endif()
    endforeach()
    set(${result} ${dir_list} PARENT_SCOPE)
endfunction()

# Now, collect the proper subdirectories of /Source
get_all_subdirectories(${CORE_SOURCE_DIR} CORE_SUBDIRS)
get_all_subdirectories(${DEV_TOOLS_SOURCE_DIR} DEV_TOOLS_SUBDIRS)
get_all_subdirectories(${EDITOR_SOURCE_DIR} EDITOR_SUBDIRS)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CORE_SOURCE_DIR}
    ${CORE_SUBDIRS}
    ${DEV_TOOLS_SOURCE_DIR}
    ${DEV_TOOLS_SUBDIRS}
    ${EDITOR_SOURCE_DIR}
    ${EDITOR_SUBDIRS}
    ${VOLK_DIR}
)

# ======================================================================================
# Link libraries to the target executable
# ======================================================================================
# Some linkers require dependent libraries to be linked after the libraries they depend on.
# In this case, the order of the libraries in the target_link_libraries() command is important.
# If EngineCore depends on Vulkan::Vulkan, linking EngineCore first ensures proper resolution.

# Link the Hydragon executable to the Core and Editor libraries.
#target_link_libraries(Hydragon PRIVATE Core Editor)    # # Uncomment when these are implemented.

# Enable runtime mode definitions
#target_compile_definitions(Hydragon PRIVATE ENABLE_EDITOR_SUPPORT=1)           # uncomment when editor is implemented.

# Add external precompiled libraries (e.g. GLFW, Vulkan, etc) to the target executable
target_link_libraries(${PROJECT_NAME} PRIVATE
    #volk::volk
    ${LIBRARIES})

# ==================================================================================
# Install Executable - for end user, tests.
# ==================================================================================
# Install binaries in Engine/Bin/<platform>/<config>
#install(TARGETS ${PROJECT_NAME}  RUNTIME DESTINATION ${INSTALL_BIN_DIR})            # uncomment to enable installs.

# ==================================================================================
# 3️⃣ Install Additional Files (Assets, Configs)
# ==================================================================================

#install(DIRECTORY ${CMAKE_SOURCE_DIR}/Assets DESTINATION ${INSTALL_ASSETS_DIR})    # uncomment to enable installs.
#install(FILES ${CMAKE_SOURCE_DIR}/config.ini DESTINATION ${INSTALL_CONFIG_DIR})    # uncomment to enable installs.

# ==================================================================================
# Optional: Custom Build Options
# ==================================================================================
option(ENABLE_DEBUG_LOGGING "Enable debug logging" ON)
if(ENABLE_DEBUG_LOGGING)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_DEBUG_LOGGING=1)
endif()



