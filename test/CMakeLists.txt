add_executable(triangle triangle.cc)
target_link_libraries(triangle glad mylib glfw.3.dylib "-framework OpenGL")
