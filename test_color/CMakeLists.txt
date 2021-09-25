set(PROGRAM_NAME color)

set(TEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/")
configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/config.h"
  )

aux_source_directory(./ SRC)
add_executable(${PROGRAM_NAME} ${SRC})
target_link_libraries(${PROGRAM_NAME} ${ALL_LIBS})