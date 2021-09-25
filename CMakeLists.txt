cmake_minimum_required(VERSION 3.0)
project(examples)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

message(STATUS "Operation system is ${CMAKE_SYSTEM}")

set(ALL_LIBS glad mylib)

if (WIN32)
    message(STATUS "Windows")
elseif (APPLE)
    message(STATUS "Apple")
    include_directories(3rd-party/glfw-3.3.4.bin.MACOS/include)
    link_directories(3rd-party/glfw-3.3.4.bin.MACOS/lib-x86_64)
    list(APPEND ALL_LIBS glfw.3.dylib "-framework OpenGL")
elseif (UNIX)
    message(STATUS "UNIX-like OS's.")
    # Maybe install libs:
    #      sudo apt-get install libgl1-mesa-dev
    # Build glfw and Install:
    #      mkdir build;cmake ..;make & make install
    # list(APPEND ALL_LIBS glfw3 GL Xrandr Xi X11 Xxf86vm pthread dl Xinerama Xcursor)
    list(APPEND ALL_LIBS glfw3 GL Xrandr Xi X11 pthread dl)
endif ()

include_directories(3rd-party/stb_image)
include_directories(3rd-party/glm)

include_directories(3rd-party/glad/include/)

include_directories(src/)

add_subdirectory(3rd-party/glad)
add_subdirectory(src)
add_subdirectory(test_triangle)
add_subdirectory(test_rectangle)
add_subdirectory(test_rgb)
add_subdirectory(test_rotate)
add_subdirectory(test_yuv)
add_subdirectory(test_sphere)
add_subdirectory(test_fullview)
add_subdirectory(test_fullview2)
add_subdirectory(test_overlying)
add_subdirectory(test_skybox)
add_subdirectory(test_3dview)
add_subdirectory(test_3dview2)