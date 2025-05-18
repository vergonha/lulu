cmake_minimum_required(VERSION 3.16)
project(LuluProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

add_executable(LuluProject
    src/main.cpp
    src/system.cpp
    src/platform.cpp
    src/logger.cpp
    src/rendering.cpp
    src/assets.cpp
    src/input.cpp
    src/camera.cpp
    src/scene.cpp
)

if(WIN32)
    target_link_libraries(LuluProject opengl32)
    target_sources(LuluProject PRIVATE src/platform/platform_windows.cpp)

elseif(APPLE)
    find_library(OpenGL_LIBRARY OpenGL)
    target_link_libraries(LuluProject ${OpenGL_LIBRARY})
    target_sources(LuluProject PRIVATE src/platform/platform_macos.cpp)


elseif(UNIX)
    find_package(X11 REQUIRED)
    find_package(OpenGL REQUIRED)

    target_link_libraries(LuluProject
        ${OPENGL_LIBRARIES}
        ${X11_LIBRARIES}
        GLX
        GLEW
        GL
        EGL
    )

    target_sources(LuluProject PRIVATE src/platform_specific/linux.cpp)

endif()
