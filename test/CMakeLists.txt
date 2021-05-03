aux_source_directory(./ SRC)
add_executable(triangle ${SRC})
target_link_libraries(triangle glad mylib glfw.3.dylib "-framework OpenGL")
