cmake_minimum_required(VERSION 3.11) # FetchContent requires CMake 3.11 or later

project(Particle-life C) # Specify C language

# FetchContent module to handle GLFW
include(FetchContent)
# Declare GLFW dependency
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        master # or a specific tag/commit
)

# Make GLFW available
FetchContent_MakeAvailable(glfw)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Add the executable
add_executable(Particle-life WIN32 src/main.c src/libraries/drawing.c)

# Link libraries
target_link_libraries(Particle-life PRIVATE glfw ${OPENGL_LIBRARIES})

# Optionally set the startup project in Visual Studio if using MSVC
if(MSVC)
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0")
        message("\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'Particle-life' as StartUp Project in Visual Studio.\n")
    else()
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Particle-life)
    endif()
endif()
