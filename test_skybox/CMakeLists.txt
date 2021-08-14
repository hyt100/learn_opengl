set(PROGRAM_NAME skybox)
aux_source_directory(./ SRC)
add_executable(${PROGRAM_NAME} ${SRC})
target_link_libraries(${PROGRAM_NAME} ${ALL_LIBS})
