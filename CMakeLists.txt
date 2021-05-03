cmake_minimum_required(VERSION 3.0)
project(examples)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(3rd-party/glfw-3.3.4.bin.MACOS/include)
link_directories(3rd-party/glfw-3.3.4.bin.MACOS/lib-x86_64)

include_directories(glad/include/)

include_directories(src/)

add_subdirectory(glad)
add_subdirectory(src)
add_subdirectory(test)
