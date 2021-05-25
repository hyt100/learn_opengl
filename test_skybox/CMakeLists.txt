set(PROGRAM_NAME skybox)
aux_source_directory(./ SRC)
add_executable(${PROGRAM_NAME} ${SRC})
target_link_libraries(${PROGRAM_NAME} glad mylib glfw.3.dylib "-framework OpenGL")
