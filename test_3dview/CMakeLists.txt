set(PROGRAM_NAME 3dview)
aux_source_directory(./ SRC)
add_executable(${PROGRAM_NAME} ${SRC})
target_link_libraries(${PROGRAM_NAME} ${ALL_LIBS})